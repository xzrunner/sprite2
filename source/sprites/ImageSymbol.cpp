#include "sprite2/ImageSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Blackboard.h"
#ifndef S2_DISABLE_STATISTICS
#include <stat/StatOverdraw.h>
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#ifdef S2_DEBUG
#include <logger.h>
#endif // S2_DEBUG
#include S2_MAT_HEADER
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/Sprite3Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/Texture.h>
#include <painting2/WndCtxStack.h>
#include <painting2/OrthoCamera.h>
#include <painting2/Pseudo3DCamera.h>

#include <assert.h>

namespace s2
{

ImageSymbol::ImageSymbol()
	: m_rotate(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

ImageSymbol::ImageSymbol(uint32_t id)
	: Symbol(id)
	, m_rotate(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

ImageSymbol::~ImageSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

int ImageSymbol::Type() const 
{ 
	return SYM_IMAGE; 
}

pt2::RenderReturn ImageSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	if (!m_tex) {
		return pt2::RENDER_NO_DATA;
	}

#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_IMAGE);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS

	if (!m_tex->IsLoadFinished()) {
		return pt2::RENDER_ON_LOADING;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

	float vertices[8];
	if (!CalcVertices(*rp_child, vertices)) {
		return pt2::RENDER_OUTSIDE;
	}

	float texcoords[8];
	int tex_id;
	if (!QueryTexcoords(!rp.IsDisableDTexC2(), texcoords, tex_id)) {
		OnQueryTexcoordsFail(dlist);
	}

//#ifndef S2_DISABLE_STATISTICS
//	const sm::ivec2& sz = Blackboard::Instance()->GetScreenSize();	
//	float area = (xmax - xmin) * (ymax - ymin) / sz.x / sz.y;
//	st::StatOverdraw::Instance()->AddArea(area);
//#endif // S2_DISABLE_STATISTICS
	
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (shader_mgr.GetShaderType() == sl::BLEND) {
		if (!dlist) {
			DrawBlend(*rp_child, vertices, texcoords, tex_id);
		}
	} else {
		//const pt2::Camera* cam = Blackboard::Instance()->GetCamera();
		//if (cam && cam->Type() == CAM_PSEUDO3D) {
		//	DrawPseudo3D(dlist, *rp_child, vertices, texcoords, tex_id);
		//} else {
			DrawOrtho(dlist, *rp_child, vertices, texcoords, tex_id);
		//}
	}

	return pt2::RENDER_OK;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn ImageSymbol::DrawNode(cooking::DisplayList* dlist,
	                               const RenderParams& rp, 
	                               const Sprite* spr,
	                               ft::FTList& ft, 
	                               int pos) const
{
//#ifndef S2_DISABLE_STATISTICS
//	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_IMAGE);
//#endif // S2_DISABLE_STATISTICS

	if (!m_tex->IsLoadFinished()) {
		return pt2::RENDER_ON_LOADING;
	}

	float vertices[8];
	if (!CalcVertices(rp, vertices)) {
		return pt2::RENDER_OUTSIDE;
	}

	float texcoords[8];
	int tex_id;
	if (!QueryTexcoords(!rp.IsDisableDTexC2(), texcoords, tex_id)) {
		OnQueryTexcoordsFail(dlist);
	}

//#ifndef S2_DISABLE_STATISTICS
//	const sm::ivec2& sz = Blackboard::Instance()->GetScreenSize();	
//	float area = (xmax - xmin) * (ymax - ymin) / sz.x / sz.y;
//	StatOverdraw::Instance()->AddArea(area);
//#endif // S2_DISABLE_STATISTICS
	
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (shader_mgr.GetShaderType() == sl::BLEND) {
		if (!dlist) {
			DrawBlend(rp, vertices, texcoords, tex_id);
		}
	} else {
		//const pt2::Camera* cam = Blackboard::Instance()->GetCamera();
		//if (cam && cam->Type() == CAM_PSEUDO3D) {
		//	DrawPseudo3D(rp, vertices, texcoords, tex_id);
		//} else {
			DrawOrtho(rp, vertices, texcoords, tex_id);
		//}
	}

	return pt2::RENDER_OK;
}
#endif // S2_DISABLE_FLATTEN

sm::vec2 ImageSymbol::GetNoTrimedSize() const
{
	if (m_tex) {
		return m_tex->GetOriSize();
	} else {
		return m_size.Size();
	}
}

void ImageSymbol::InitTex(const std::shared_ptr<pt2::Texture>& tex, const sm::i16_rect& region)
{
	m_tex = tex;
	m_region = region;
}

bool ImageSymbol::IsLoaded() const
{
	return m_tex && m_tex->IsLoadFinished();
}

sm::rect ImageSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return m_size;
}

void ImageSymbol::DrawBlend(const RenderParams& rp, float* vertices, const float* texcoords, int tex_id) const
{
	if (!m_tex) {
		return;
	}

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(shader_mgr.GetShader(sl::BLEND));
	shader->SetColor(rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR());

	for (int i = 0, ptr = 0; i < 4; ++i) {
		vertices[ptr++] += rp.vertex_offset.x;
		vertices[ptr++] += rp.vertex_offset.y;
	}

	sm::vec2 vertices_scr[4];
	vertices_scr[0] = rp.mt * sm::vec2(m_size.xmin, m_size.ymin);
	vertices_scr[1] = rp.mt * sm::vec2(m_size.xmax, m_size.ymin);
	vertices_scr[2] = rp.mt * sm::vec2(m_size.xmax, m_size.ymax);
	vertices_scr[3] = rp.mt * sm::vec2(m_size.xmin, m_size.ymax);

	auto cam = Blackboard::Instance()->GetCamera();
	std::shared_ptr<const pt2::OrthoCamera> ocam = nullptr;
	if (cam) {
		assert(cam->Type() == pt2::CAM_ORTHO2D);
		ocam = std::dynamic_pointer_cast<const pt2::OrthoCamera>(cam);
	}
	
	sm::vec2 tex_coords_base[4];
	const sm::ivec2& screen_sz = Blackboard::Instance()->GetScreenSize();
	if (cam) {
		for (int i = 0; i < 4; ++i)  {
			tex_coords_base[i] = ocam->TransPosProjectToScreen(vertices_scr[i], screen_sz.x, screen_sz.y);
		}
	} else {
		for (int i = 0; i < 4; ++i)  {
			tex_coords_base[i].x = screen_sz.x * 0.5f + vertices_scr[i].x;
			tex_coords_base[i].y = screen_sz.y * 0.5f - vertices_scr[i].y;
		}
	}
	for (int i = 0; i < 4; ++i) 
	{
		tex_coords_base[i].y = screen_sz.y - 1 - tex_coords_base[i].y;
		tex_coords_base[i].x /= screen_sz.x;
		tex_coords_base[i].y /= screen_sz.y;
	}

	int screen_cache_texid = Blackboard::Instance()->GetScreenCacheTexID();
	if (screen_cache_texid < 0) {
		return;
	}

	shader->Draw(vertices, texcoords, &tex_coords_base[0].x, tex_id, screen_cache_texid);
}

void ImageSymbol::DrawOrtho(cooking::DisplayList* dlist, const RenderParams& rp, const float* vertices, const float* texcoords, int tex_id) const
{
	sl::ShaderType shader;
#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader = shader_mgr.GetShaderType();
#else
	assert(dlist);
	shader = static_cast<sl::ShaderType>(dlist->GetShaderType());
#endif // S2_DISABLE_DEFERRED

//	auto& col = rp.color;
	switch (shader)
	{
	case sl::FILTER:
		{
#ifdef S2_DISABLE_DEFERRED
		auto shader = static_cast<sl::FilterShader*>(shader_mgr.GetShader(sl::FILTER));
		shader->SetColor(rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR());
		shader->Draw(vertices, texcoords, tex_id);
#else
		cooking::set_color_filter(dlist, rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR());
		cooking::draw_quad_filter(dlist, vertices, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED
		}
		break;
	case sl::SPRITE2:
		{
#ifdef S2_DISABLE_DEFERRED
		auto shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
		shader->SetColor(rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR());
		shader->SetColorMap(rp.col_map.rmap.ToABGR(), rp.col_map.gmap.ToABGR(), rp.col_map.bmap.ToABGR());
		shader->DrawQuad(vertices, texcoords, tex_id);
#else
		cooking::set_color_sprite(dlist, rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR(),
			rp.col_map.rmap.ToABGR(), rp.col_map.gmap.ToABGR(), rp.col_map.bmap.ToABGR());
		cooking::draw_quad_sprite(dlist, vertices, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED
		}
		break;
	}
}

void ImageSymbol::DrawPseudo3D(cooking::DisplayList* dlist, const RenderParams& rp, const float* vertices, const float* texcoords, int tex_id) const
{
#ifndef PT2_DISABLE_CAMERA25

	auto cam = Blackboard::Instance()->GetCamera();
	assert(cam && cam->Type() == pt2::CAM_PSEUDO3D);
	auto pcam = std::dynamic_pointer_cast<const pt2::Pseudo3DCamera>(cam);

	float z[4];
	rp.camera.CalculateZ(pcam->GetAngle(), vertices, z);

	CU_VEC<sm::vec3> _vertices;
	_vertices.push_back(sm::vec3(vertices[0], vertices[1], z[0]));
	_vertices.push_back(sm::vec3(vertices[2], vertices[3], z[1]));
	_vertices.push_back(sm::vec3(vertices[4], vertices[5], z[2]));
	_vertices.push_back(sm::vec3(vertices[0], vertices[1], z[0]));
	_vertices.push_back(sm::vec3(vertices[4], vertices[4], z[2]));
	_vertices.push_back(sm::vec3(vertices[6], vertices[7], z[3]));

	CU_VEC<sm::vec2> _texcoords;
	_texcoords.push_back(sm::vec2(texcoords[0], texcoords[1]));
	_texcoords.push_back(sm::vec2(texcoords[2], texcoords[3]));
	_texcoords.push_back(sm::vec2(texcoords[4], texcoords[5]));
	_texcoords.push_back(sm::vec2(texcoords[0], texcoords[1]));
	_texcoords.push_back(sm::vec2(texcoords[4], texcoords[5]));
	_texcoords.push_back(sm::vec2(texcoords[6], texcoords[7]));

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE3);
	sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(shader_mgr.GetShader(sl::SPRITE3));
	shader->SetColor(rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR());
	shader->SetColorMap(rp.col_map.rmap.ToABGR(), rp.col_map.gmap.ToABGR(), rp.col_map.bmap.ToABGR());
	shader->Draw(&_vertices[0].x, &_texcoords[0].x, tex_id);
#else
	cooking::change_shader(dlist, sl::SPRITE3);
	cooking::set_color_sprite3(dlist, rp.col_common.mul.ToABGR(), rp.col_common.add.ToABGR(),
		rp.col_map.rmap.ToABGR(), rp.col_map.gmap.ToABGR(), rp.col_map.bmap.ToABGR());
	cooking::draw_quad_sprite3(dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED

#endif // PT2_DISABLE_CAMERA25
}

bool ImageSymbol::CalcVertices(const RenderParams& rp, float* vertices) const
{
	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax = -FLT_MAX, ymax = -FLT_MAX;

	const float* mt = rp.mt.x;

	float x, y;

	float* ptr_dst = &vertices[0];

	x = (m_size.xmin * mt[0] + m_size.ymin * mt[2]) + mt[4];
	y = (m_size.xmin * mt[1] + m_size.ymin * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (m_size.xmax * mt[0] + m_size.ymin * mt[2]) + mt[4];
	y = (m_size.xmax * mt[1] + m_size.ymin * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (m_size.xmax * mt[0] + m_size.ymax * mt[2]) + mt[4];
	y = (m_size.xmax * mt[1] + m_size.ymax * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (m_size.xmin * mt[0] + m_size.ymax * mt[2]) + mt[4];
	y = (m_size.xmin * mt[1] + m_size.ymax * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	if (rp.IsViewRegionValid()) {
		const sm::rect& vr = rp.GetViewRegion();
		if (xmax <= vr.xmin || xmin >= vr.xmax ||
			ymax <= vr.ymin || ymin >= vr.ymax) {
			return false;
		}
	}

	return true;
}

}