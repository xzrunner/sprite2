#include "DrawMesh.h"
#include "S2_Mesh.h"
#include "S2_RVG.h"
#include "Color.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "ImageSymbol.h"
#include "S2_Texture.h"

#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>

#include <set>
#include <vector>

#include <assert.h>

namespace s2
{

static Color RED	(204, 51, 102, 128);
static Color GREEN	(102, 204, 51, 128);
static Color BLUE	(102, 51, 204, 128);

void DrawMesh::DrawInfoUV(const Mesh* mesh, const S2_MAT* mt)
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	mesh->DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return;
	}
	
	float w = mesh->GetWidth(),
		  h = mesh->GetHeight();

	// lines
	RVG::SetColor(RED);
	std::vector<sm::vec2> lines(3);
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j].x = (texcoords[triangles[i]].x - 0.5f) * w;
			lines[j].y = (texcoords[triangles[i]].y - 0.5f) * h;
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		RVG::Polyline(lines, true);
	}

	// points
	RVG::SetColor(BLUE);
	for (int i = 0, n = texcoords.size(); i < n; ++i) {
		sm::vec2 p;
		p.x = (texcoords[i].x - 0.5f) * w;
		p.y = (texcoords[i].y - 0.5f) * h;
		if (mt) {
			p = *mt * p;
		}
		RVG::Circle(p, mesh->GetNodeRadius(), true);
	}
}

void DrawMesh::DrawInfoXY(const Mesh* mesh, const S2_MAT* mt)
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	mesh->DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return;
	}

	// lines
	RVG::SetColor(RED);
	std::vector<sm::vec2> lines(3);
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j] = vertices[triangles[i]];
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		RVG::Polyline(lines, true);
	}

	// points
	RVG::SetColor(BLUE);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sm::vec2 p = vertices[i];
		if (mt) {
			p = *mt * p;
		}
		RVG::Circle(p, mesh->GetNodeRadius(), true);
	}
}

void DrawMesh::DrawTexture(const Mesh* mesh, const RenderParams& rp, const Symbol* base_sym)
{
	const Symbol* sym = base_sym ? base_sym : mesh->GetBaseSymbol();
	if (sym->Type() == SYM_IMAGE) {
		DrawOnePass(mesh, rp, sym);
	} else {
		DrawTwoPass(mesh, rp, sym);
	}
}

void DrawMesh::DrawOnlyMesh(const Mesh* mesh, const S2_MAT& mt, int tex_id)
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	mesh->DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);

	int w = RenderTargetMgr::Instance()->WIDTH,
		h = RenderTargetMgr::Instance()->HEIGHT;
	float ori_w = mesh->GetWidth(),
		  ori_h = mesh->GetHeight();
	for (int i = 0, n = triangles.size(); i < n; )
	{
		sm::vec2 _vertices[4], _texcoords[4];
		for (int j = 0; j < 3; ++j, ++i)
		{
			int idx = triangles[i];
			_vertices[j] = mt * vertices[idx];
			_texcoords[j].x = (texcoords[idx].x * ori_w - ori_w * 0.5f + w * 0.5f) / w;
			_texcoords[j].y = (texcoords[idx].y * ori_h - ori_h * 0.5f + h * 0.5f) / h;
		}
		_vertices[3] = _vertices[2];
		_texcoords[3] = _texcoords[2];

		shader->DrawQuad(&_vertices[0].x, &_texcoords[0].x, tex_id);
	}
}

static void draw_sprite2(const float* positions, const float* texcoords, int tex_id)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	assert(mgr->GetShaderType() == sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->DrawQuad(positions, texcoords, tex_id);
}

static void draw_filter(const float* positions, const float* texcoords, int tex_id)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	assert(mgr->GetShaderType() == sl::FILTER);
	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
	shader->Draw(positions, texcoords, tex_id);
}

void DrawMesh::DrawOnePass(const Mesh* mesh, const RenderParams& rp, const Symbol* sym)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::ShaderType type = mgr->GetShaderType();
	if (type != sl::SPRITE2 && type != sl::FILTER) {
		return;
	}

	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	mesh->DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return;
	}

	assert(sym->Type() == SYM_IMAGE);
	const ImageSymbol* img_sym = VI_DOWNCASTING<const ImageSymbol*>(sym);
	float src_texcoords[8];
	int tex_id;
	img_sym->QueryTexcoords(!rp.IsDisableDTexC2(), src_texcoords, tex_id);

	float x = src_texcoords[0], y = src_texcoords[1];
	float w = src_texcoords[4] - src_texcoords[0],
		  h = src_texcoords[5] - src_texcoords[1];	

	void (*draw)(const float* positions, const float* texcoords, int tex_id) = NULL;

	if (type == sl::SPRITE2)
	{
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
		shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());
		draw = draw_sprite2;
	}
	else if (type == sl::FILTER)
	{
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
		draw = draw_filter;
	}

	// 3 2    1 0
	// 0 1 or 2 3 
	if ((w > 0 && h > 0) || (w < 0 && h < 0)) 
	{
		for (int i = 0, n = triangles.size(); i < n; )
		{
			sm::vec2 _vertices[4], _texcoords[4];
			for (int j = 0; j < 3; ++j, ++i)
			{
				int idx = triangles[i];
				_vertices[j] = rp.mt * vertices[idx];
				_texcoords[j].x = x + w * texcoords[idx].x;
				_texcoords[j].y = y + h * texcoords[idx].y;
			}
			_vertices[3] = _vertices[2];
			_texcoords[3] = _texcoords[2];

			draw(&_vertices[0].x, &_texcoords[0].x, tex_id);
		}
	}
	// 0 3
	// 1 2
	else if (w > 0 && h < 0)
	{
		for (int i = 0, n = triangles.size(); i < n; )
		{
			sm::vec2 _vertices[4], _texcoords[4];
			for (int j = 0; j < 3; ++j, ++i)
			{
				int idx = triangles[i];
				_vertices[j] = rp.mt * vertices[idx];
				_texcoords[j].x = x + w * texcoords[idx].y;
				_texcoords[j].y = y + h * texcoords[idx].x;
			}
			_vertices[3] = _vertices[2];
			_texcoords[3] = _texcoords[2];

			draw(&_vertices[0].x, &_texcoords[0].x, tex_id);
		}
	}
	else
	{
		assert(0);
	}
}

void DrawMesh::DrawTwoPass(const Mesh* mesh, const RenderParams& rp, const Symbol* sym)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return;
	}
	
	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	DrawMesh2RT(rt, rp, sym);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();

	DrawRT2Screen(rt, mesh, rp.mt);

	RT->Return(rt);
}

void DrawMesh::DrawMesh2RT(RenderTarget* rt, const RenderParams& rp, const Symbol* sym)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams rp_child(rp);
	rp_child.mt.Identity();
	DrawNode::Draw(sym, rp_child);

	mgr->FlushShader();

	rt->Unbind();
}

void DrawMesh::DrawRT2Screen(RenderTarget* rt, const Mesh* mesh, const S2_MAT& mt)
{
	DrawOnlyMesh(mesh, mt, rt->GetTexID());
}

}