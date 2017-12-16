#include "sprite2/ImageSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/Texture.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Blackboard.h"
#include "sprite2/OrthoCamera.h"
#include "sprite2/Pseudo3DCamera.h"
#include "sprite2/RenderContext.h"
#include "sprite2/RenderCtxStack.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatOverdraw.h"
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#ifdef S2_DEBUG
#include <logger.h>
#endif // S2_DEBUG
#include S2_MAT_HEADER
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/Sprite3Shader.h>
#include <cooking/Facade.h>
#include <cooking/DisplayList.h>

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

RenderReturn ImageSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	if (!m_tex) {
		return RENDER_NO_DATA;
	}

#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_IMAGE);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS

	if (!m_tex->IsLoadFinished()) {
		return RENDER_ON_LOADING;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}

	float vertices[8];
	if (!CalcVertices(*rp_child, vertices)) {
		return RENDER_OUTSIDE;
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
	
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (mgr->GetShaderType() == sl::BLEND) {
		if (!dlist) {
			DrawBlend(*rp_child, vertices, texcoords, tex_id);
		}
	} else {
		//const Camera* cam = Blackboard::Instance()->GetCamera();
		//if (cam && cam->Type() == CAM_PSEUDO3D) {
		//	DrawPseudo3D(dlist, *rp_child, vertices, texcoords, tex_id);
		//} else {
			if (dlist) {
				DrawOrthoDeferred(dlist, *rp_child, vertices, texcoords, tex_id);
			} else {
				DrawOrtho(*rp_child, vertices, texcoords, tex_id);
			}
		//}
	}

	return RENDER_OK;
}

#ifndef S2_DISABLE_FLATTEN
RenderReturn ImageSymbol::DrawNode(cooking::DisplayList* dlist,
	                               const RenderParams& rp, 
	                               const Sprite* spr,
	                               ft::FTList& ft, 
	                               int pos) const
{
//#ifndef S2_DISABLE_STATISTICS
//	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_IMAGE);
//#endif // S2_DISABLE_STATISTICS

	if (!m_tex->IsLoadFinished()) {
		return RENDER_ON_LOADING;
	}

	float vertices[8];
	if (!CalcVertices(rp, vertices)) {
		return RENDER_OUTSIDE;
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
	
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (mgr->GetShaderType() == sl::BLEND) {
		if (!dlist) {
			DrawBlend(rp, vertices, texcoords, tex_id);
		}
	} else {
		//const Camera* cam = Blackboard::Instance()->GetCamera();
		//if (cam && cam->Type() == CAM_PSEUDO3D) {
		//	DrawPseudo3D(rp, vertices, texcoords, tex_id);
		//} else {
			if (dlist) {
				DrawOrthoDeferred(dlist, rp, vertices, texcoords, tex_id);
			} else {
				DrawOrtho(rp, vertices, texcoords, tex_id);
			}
		//}
	}

	return RENDER_OK;
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

void ImageSymbol::InitTex(const std::shared_ptr<Texture>& tex, const sm::i16_rect& region)
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

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());

	for (int i = 0, ptr = 0; i < 4; ++i) {
		vertices[ptr++] += rp.vertex_offset.x;
		vertices[ptr++] += rp.vertex_offset.y;
	}

	sm::vec2 vertices_scr[4];
	vertices_scr[0] = rp.mt * sm::vec2(m_size.xmin, m_size.ymin);
	vertices_scr[1] = rp.mt * sm::vec2(m_size.xmax, m_size.ymin);
	vertices_scr[2] = rp.mt * sm::vec2(m_size.xmax, m_size.ymax);
	vertices_scr[3] = rp.mt * sm::vec2(m_size.xmin, m_size.ymax);

	const Camera* cam = Blackboard::Instance()->GetCamera();
	const OrthoCamera* ocam = nullptr;
	if (cam) {
		assert(cam->Type() == CAM_ORTHO2D);
		ocam = static_cast<const OrthoCamera*>(cam);
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

void ImageSymbol::DrawOrtho(const RenderParams& rp, const float* vertices, const float* texcoords, int tex_id) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
// 	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
// 	shader->SetColor(trans.color);
	if (mgr->GetShaderType() == sl::FILTER) {
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
		shader->Draw(vertices, texcoords, tex_id);
	} else if (mgr->GetShaderType() == sl::SPRITE2) {
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
//#ifdef S2_DEBUG
//		if (!IsProxyImg()) {
//			int pkg_id = (GetID() >> 20);
//			if (pkg_id > 128 && pkg_id < 384) {
//				shader->SetColor(0x88000088, 0);
////				LOGI("not preloaded char, pkg %d", pkg_id);
//			} else {
//				shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
//			}
//		} else {
//			shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
//		}
//#else
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
//#endif // S2_DEBUG
		shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());
		shader->DrawQuad(vertices, texcoords, tex_id);
	}
}

void ImageSymbol::DrawPseudo3D(cooking::DisplayList* dlist, const RenderParams& rp, const float* vertices, const float* texcoords, int tex_id) const
{
	const Camera* cam = Blackboard::Instance()->GetCamera();
	assert(cam && cam->Type() == CAM_PSEUDO3D);
	const Pseudo3DCamera* pcam = static_cast<const Pseudo3DCamera*>(cam);

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

	auto& col = rp.color;
#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE3);
	sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(mgr->GetShader(sl::SPRITE3));
	shader->SetColor(col.GetMulABGR(), col.GetAddABGR());
	shader->SetColorMap(col.GetRMapABGR(), col.GetGMapABGR(), col.GetBMapABGR());
	shader->Draw(&_vertices[0].x, &_texcoords[0].x, tex_id);
#else
	cooking::change_shader(dlist, sl::SPRITE3);
	cooking::set_color_sprite3(dlist, col.GetMulABGR(), col.GetAddABGR(), 
		col.GetRMapABGR(), col.GetGMapABGR(), col.GetBMapABGR());
	cooking::draw_quad_sprite3(dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED
}

void ImageSymbol::DrawOrthoDeferred(cooking::DisplayList* dlist, const RenderParams& rp, 
									const float* vertices, const float* texcoords, int tex_id) const
{
	auto mgr = sl::ShaderMgr::Instance();
	switch (mgr->GetShaderType())
	{
	case sl::SPRITE2:
		{
			const RenderColor& col = rp.color;
			uint32_t col_mul = col.GetMulABGR(), 
			         col_add = col.GetAddABGR();
			uint32_t col_rmap = col.GetRMapABGR(),
		             col_gmap = col.GetGMapABGR(),
			         col_bmap = col.GetBMapABGR();
			cooking::set_color_sprite(dlist, col_mul, col_add, col_rmap, col_gmap, col_bmap);
			cooking::draw_quad_sprite(dlist, vertices, texcoords, tex_id);
		}
		break;
	case sl::FILTER:
		{
			const RenderColor& col = rp.color;
			uint32_t col_mul = col.GetMulABGR(), 
			         col_add = col.GetAddABGR();
			cooking::set_color_filter(dlist, col_mul, col_add);
			cooking::draw_quad_filter(dlist, vertices, texcoords, tex_id);
		}
		break;
	}
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