#include "DrawMesh.h"
#include "MeshTriangle.h"
#include "Mesh.h"
#include "S2_RVG.h"
#include "Color.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "RenderTargetMgr.h"
#include "RenderTarget.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "ImageSymbol.h"
#include "Texture.h"

#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

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
	std::set<sm::vec2, sm::Vector2Cmp> unique;
	std::vector<sm::vec2> tmp(3);
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	float w = mesh->GetWidth(),
		  h = mesh->GetHeight();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * w;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * h;
			if (mt) {
				tmp[i] = *mt * tmp[i];
			}
			unique.insert(tmp[i]);
		}
		RVG::SetColor(RED);
		RVG::Polyline(tmp, true);
	}
	std::vector<sm::vec2> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	RVG::SetColor(BLUE);
	RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void DrawMesh::DrawInfoXY(const Mesh* mesh, const S2_MAT* mt)
{
	std::set<sm::vec2, sm::Vector2Cmp> unique;
	std::vector<sm::vec2> tmp(3);
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			if (mt) {
				tmp[i] = *mt * tmp[i];
			}
			unique.insert(tmp[i]);
		}
		RVG::SetColor(RED);
		RVG::Polyline(tmp, true);
	}
	std::vector<sm::vec2> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	RVG::SetColor(BLUE);
	RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void DrawMesh::DrawTexture(const Mesh* mesh, const RenderParams& params, const Symbol* base_sym)
{
	const Symbol* sym = base_sym ? base_sym : mesh->GetBaseSymbol();
	if (sym->Type() == SYM_IMAGE) {
		DrawOnePass(mesh, params, sym);
	} else {
		DrawTwoPass(mesh, params, sym);
	}
}

void DrawMesh::DrawOnlyMesh(const Mesh* mesh, const S2_MAT& mt, int texid)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);

	int w = RenderTargetMgr::Instance()->WIDTH,
		h = RenderTargetMgr::Instance()->HEIGHT;
	float ori_w = mesh->GetWidth(),
		  ori_h = mesh->GetHeight();
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = tris[i];
		sm::vec2 vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = mt * tri->nodes[i]->xy;
			texcoords[i].x = (tri->nodes[i]->uv.x * ori_w - ori_w * 0.5f + w * 0.5f) / w;
			texcoords[i].y = (tri->nodes[i]->uv.y * ori_h - ori_h * 0.5f + h * 0.5f) / h;
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
	}
}

void DrawMesh::DrawOnePass(const Mesh* mesh, const RenderParams& params, const Symbol* sym)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (mgr->GetShaderType() != sl::SPRITE2) {
		return;
	}

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(params.color.GetMul().ToABGR(), params.color.GetAdd().ToABGR());
	shader->SetColorMap(params.color.GetMapR().ToABGR(),params.color.GetMapG().ToABGR(), params.color.GetMapB().ToABGR());

	assert(sym->Type() == SYM_IMAGE);
	const ImageSymbol* img_sym = dynamic_cast<const ImageSymbol*>(sym);
	float src_texcoords[8];
	int texid;
	img_sym->QueryTexcoords(src_texcoords, texid);
	
	float x = src_texcoords[0], y = src_texcoords[1];
	float w = src_texcoords[4] - src_texcoords[0],
		  h = src_texcoords[5] - src_texcoords[1];
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	if (h < 0)
	{
		for (int i = 0, n = tris.size(); i < n; ++i)
		{
			MeshTriangle* tri = tris[i];
			sm::vec2 vertices[4], texcoords[4];
			for (int i = 0; i < 3; ++i)
			{
				vertices[i] = params.mt * tri->nodes[i]->xy;
				texcoords[i].x = x + w * tri->nodes[i]->uv.y;
				texcoords[i].y = y + h * tri->nodes[i]->uv.x;
			}
			vertices[3] = vertices[2];
			texcoords[3] = texcoords[2];

			shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
		}
	}
	else
	{
		for (int i = 0, n = tris.size(); i < n; ++i)
		{
			MeshTriangle* tri = tris[i];
			sm::vec2 vertices[4], texcoords[4];
			for (int i = 0; i < 3; ++i)
			{
				vertices[i] = params.mt * tri->nodes[i]->xy;
				texcoords[i].x = x + w * tri->nodes[i]->uv.x;
				texcoords[i].y = y + h * tri->nodes[i]->uv.y;
			}
			vertices[3] = vertices[2];
			texcoords[3] = texcoords[2];

			shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
		}
	}
}

void DrawMesh::DrawTwoPass(const Mesh* mesh, const RenderParams& params, const Symbol* sym)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return;
	}
	
	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	DrawMesh2RT(rt, params, sym);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();

	DrawRT2Screen(rt, mesh, params.mt);

	RT->Return(rt);
}

void DrawMesh::DrawMesh2RT(RenderTarget* rt, const RenderParams& params, const Symbol* sym)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams _params = params;
	_params.mt.Identity();
	DrawNode::Draw(sym, _params);

	mgr->FlushShader();

	rt->Unbind();
}

void DrawMesh::DrawRT2Screen(RenderTarget* rt, const Mesh* mesh, const S2_MAT& mt)
{
	DrawOnlyMesh(mesh, mt, rt->GetTexID());
}

}