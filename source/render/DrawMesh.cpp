#include "sprite2/DrawMesh.h"
#include "sprite2/Mesh.h"
#include "sprite2/RVG.h"
#include "sprite2/Color.h"
#include "sprite2/RenderCtxStack.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#include "sprite2/RenderScissor.h"
#include "sprite2/RenderTargetMgr.h"
#include "sprite2/RenderTarget.h"
#include "sprite2/Symbol.h"
#include "sprite2/SymType.h"
#include "sprite2/ImageSymbol.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS

#include <painting2/Texture.h>
#include <memmgr/Allocator.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>
#include <cu/cu_stl.h>
#include <cooking/DisplayList.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED

#include <assert.h>

namespace s2
{

static Color RED	(204, 51, 102, 128);
static Color GREEN	(102, 204, 51, 128);
static Color BLUE	(102, 51, 204, 128);

RenderReturn DrawMesh::DrawInfoUV(cooking::DisplayList* dlist, const Mesh& mesh, const S2_MAT* mt)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}
	
	float w = mesh.GetWidth(),
		  h = mesh.GetHeight();

	// lines
	RVG::SetColor(RED);
	CU_VEC<sm::vec2> lines;
	lines.resize(3);
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j].x = (texcoords[triangles[i]].x - 0.5f) * w;
			lines[j].y = (texcoords[triangles[i]].y - 0.5f) * h;
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		RVG::Polyline(dlist, lines, true);
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
		RVG::Circle(dlist, p, mesh.GetNodeRadius(), true);
	}

	return RENDER_OK;
}

RenderReturn DrawMesh::DrawInfoXY(cooking::DisplayList* dlist, const Mesh& mesh, const S2_MAT* mt)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

	// lines
	RVG::SetColor(RED);
	CU_VEC<sm::vec2> lines;
	lines.resize(3);
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j] = vertices[triangles[i]];
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		RVG::Polyline(dlist, lines, true);
	}

	// points
	RVG::SetColor(BLUE);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sm::vec2 p = vertices[i];
		if (mt) {
			p = *mt * p;
		}
		RVG::Circle(dlist, p, mesh.GetNodeRadius(), true);
	}

	return RENDER_OK;
}

RenderReturn DrawMesh::DrawTexture(cooking::DisplayList* dlist, const Mesh& mesh, 
	                               const RenderParams& rp, const SymConstPtr& base_sym)
{
	RenderReturn ret = RENDER_OK;
	auto& sym = base_sym ? base_sym : mesh.GetBaseSymbol();
	if (sym->Type() == SYM_IMAGE) 
	{
	 	auto img_sym = S2_VI_PTR_DOWN_CAST<const ImageSymbol>(sym);
	 	if(!img_sym->GetTexture()->IsLoadFinished()) {
	 		return RENDER_ON_LOADING;
	 	}
	 	float texcoords[8];
	 	int tex_id;
	 	if (!img_sym->QueryTexcoords(!rp.IsDisableDTexC2(), texcoords, tex_id)) {
	 		img_sym->OnQueryTexcoordsFail(dlist);
	 	}
		ret = DrawOnePass(dlist, mesh, rp, texcoords, tex_id);
	} 
	else 
	{
		ret = DrawTwoPass(dlist, mesh, rp, *sym);

		//////////////////////////////////////////////////////////////////////////

//  		int tex_id;
//  		const float* texcoords = DrawNode::DTexQuerySym(sym, tex_id);
//  		if (texcoords) {
//  			DrawOnePass(mesh, rp, texcoords, tex_id);
//  		} else {
//  			DrawNode::DTexCacheSym(sym);
// 			DrawTwoPass(mesh, rp, sym);
// 		}
	}
	return ret;
}

RenderReturn DrawMesh::DrawOnlyMesh(cooking::DisplayList* dlist, const Mesh& mesh, const S2_MAT& mt, int tex_id)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->SetShader(sl::SPRITE2);

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
#else
	cooking::change_shader(dlist, sl::SPRITE2);
	cooking::set_color_sprite(dlist, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
#endif // S2_DISABLE_DEFERRED

	int w = RenderTargetMgr::Instance()->WIDTH,
		h = RenderTargetMgr::Instance()->HEIGHT;
	float ori_w = mesh.GetWidth(),
		  ori_h = mesh.GetHeight();
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

#ifdef S2_DISABLE_DEFERRED
		shader->DrawQuad(&_vertices[0].x, &_texcoords[0].x, tex_id);
#else
		cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED
	}

	return RENDER_OK;
}

static void draw_sprite2(cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id)
{
#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	assert(mgr->GetShaderType() == sl::SPRITE2);

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->DrawQuad(positions, texcoords, tex_id);
#else
	cooking::draw_quad_sprite(dlist, positions, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED
}

static void draw_filter(cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id)
{
#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	assert(mgr->GetShaderType() == sl::FILTER);

	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
	shader->Draw(positions, texcoords, tex_id);
#else
	cooking::draw_quad_filter(dlist, positions, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED
}

RenderReturn DrawMesh::DrawOnePass(cooking::DisplayList* dlist, const Mesh& mesh, const RenderParams& rp, const float* src_texcoords, int tex_id)
{
	sl::ShaderType shader_type;
#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	shader_type = mgr->GetShaderType();
	if (shader_type != sl::SPRITE2 && shader_type != sl::FILTER) {
		return RENDER_NO_DATA;
	}
#else
	int _shader_type = dlist->GetShaderType();
	assert(_shader_type >= 0);
	shader_type = static_cast<sl::ShaderType>(_shader_type);
	if (shader_type != sl::SPRITE2 && shader_type != sl::FILTER) {
		return RENDER_NO_DATA;
	}
#endif // S2_DISABLE_DEFERRED

	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

	float x = src_texcoords[0], y = src_texcoords[1];
	float w = src_texcoords[4] - src_texcoords[0],
		  h = src_texcoords[5] - src_texcoords[1];	

	void (*draw)(cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id) = nullptr;

	switch (shader_type)
	{
		case sl::SPRITE2:
		{
			auto& col = rp.color;
#ifdef S2_DISABLE_DEFERRED
			sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
			shader->SetColor(col.GetMulABGR(), col.GetAddABGR());
			shader->SetColorMap(col.GetRMapABGR(), col.GetGMapABGR(), col.GetBMapABGR());
#else
			cooking::set_color_sprite(dlist, col.GetMulABGR(), col.GetAddABGR(),
				col.GetRMapABGR(), col.GetGMapABGR(), col.GetBMapABGR());
#endif // S2_DISABLE_DEFERRED
			draw = draw_sprite2;
		}
			break;
		case sl::FILTER:
		{
			auto& col = rp.color;
#ifdef S2_DISABLE_DEFERRED
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetColor(col.GetMulABGR(), col.GetAddABGR());
#else
			cooking::set_color_filter(dlist, col.GetMulABGR(), col.GetAddABGR());
#endif // S2_DISABLE_DEFERRED
			draw = draw_filter;
		}
			break;
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

			draw(dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
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

			draw(dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
		}
	}
	else
	{
		assert(0);
	}

	return RENDER_OK;
}

RenderReturn DrawMesh::DrawTwoPass(cooking::DisplayList* dlist, const Mesh& mesh, const RenderParams& rp, const Symbol& sym)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::MESH);
#endif // S2_DISABLE_STATISTICS

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	ret |= DrawMesh2RT(dlist, rt, rp, sym);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	ret |= DrawRT2Screen(dlist, rt, mesh, rp.mt);

	RT->Return(rt);

	return ret;
}

RenderReturn DrawMesh::DrawMesh2RT(cooking::DisplayList* dlist, RenderTarget* rt, const RenderParams& rp, const Symbol& sym)
{
	rt->Bind();

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);
#else
	cooking::render_clear(dlist, 0);
#endif // S2_DISABLE_DEFERRED

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->mt.Identity();
	// make empty
	rp_child->SetViewRegion(sm::rect());

	RenderReturn ret = DrawNode::Draw(sym, *rp_child);

#ifdef S2_DISABLE_DEFERRED
	mgr->FlushShader();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	rt->Unbind();

	return ret;
}

RenderReturn DrawMesh::DrawRT2Screen(cooking::DisplayList* dlist, RenderTarget* rt, const Mesh& mesh, const S2_MAT& mt)
{
	return DrawOnlyMesh(dlist, mesh, mt, rt->GetTexID());
}

}