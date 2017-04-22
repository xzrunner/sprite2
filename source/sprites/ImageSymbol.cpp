#include "ImageSymbol.h"
#include "SymType.h"
#include "S2_Texture.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "Blackboard.h"
#include "OrthoCamera.h"
#include "Pseudo3DCamera.h"
#include "S2_RenderContext.h"
#include "RenderCtxStack.h"
#include "Flatten.h"
#include "FlattenParams.h"

#include S2_MAT_HEADER
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/Sprite3Shader.h>

#include <assert.h>

namespace s2
{

ImageSymbol::ImageSymbol()
	: m_tex(NULL)
	, m_rotate(false)
{
}

ImageSymbol::ImageSymbol(uint32_t id)
	: Symbol(id)
	, m_tex(NULL)
	, m_rotate(false)
{
}

ImageSymbol::~ImageSymbol()
{
	if (m_tex) {
		m_tex->RemoveReference();
	}
}

int ImageSymbol::Type() const 
{ 
	return SYM_IMAGE; 
}

void ImageSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_tex->IsLoadFinished()) {
		return;
	}

	RenderParams rp_child(rp);
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax =-FLT_MAX, ymax =-FLT_MAX;
	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(m_size.xmin, m_size.ymin);
	vertices[1] = sm::vec2(m_size.xmax, m_size.ymin);
	vertices[2] = sm::vec2(m_size.xmax, m_size.ymax);
	vertices[3] = sm::vec2(m_size.xmin, m_size.ymax);
	for (int i = 0; i < 4; ++i) 
	{
		sm::vec2 pos = rp_child.mt * vertices[i];
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
		vertices[i] = pos;
	}

	if (rp.view_region.IsValid() && 
		(xmax <= rp.view_region.xmin || xmin >= rp.view_region.xmax ||
		 ymax <= rp.view_region.ymin || ymin >= rp.view_region.ymax)) {
		return;
	}

	float texcoords[8];
	int tex_id;
	if (!QueryTexcoords(!rp.IsDisableDTexC2(), texcoords, tex_id)) {
		return;
	}
	
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (mgr->GetShaderType() == sl::BLEND) {
		DrawBlend(rp_child, vertices, texcoords, tex_id);
	} else {
		const Camera* cam = Blackboard::Instance()->GetCamera();
		if (cam && cam->Type() == CAM_PSEUDO3D) {
			DrawPseudo3D(rp_child, vertices, texcoords, tex_id);
		} else {
			DrawOrtho(rp_child, vertices, texcoords, tex_id);
		}
	}
}

void ImageSymbol::Flattening(const FlattenParams& fp, Flatten& ft) const
{
	Flatten::Quad quad;
	QueryTexcoords(false, &quad.texcoords[0].x, quad.tex_id);
	quad.sym_id = GetID();

	sm::rect sz = GetBounding();
	const S2_MAT& mt = fp.GetMat();
	quad.vertices[0] = mt * sm::vec2(sz.xmin, sz.ymin);
	quad.vertices[1] = mt * sm::vec2(sz.xmax, sz.ymin);
	quad.vertices[2] = mt * sm::vec2(sz.xmax, sz.ymax);
	quad.vertices[3] = mt * sm::vec2(sz.xmin, sz.ymax);

	ft.Add(quad);
}

sm::vec2 ImageSymbol::GetNoTrimedSize() const
{
	if (m_tex) {
		return m_tex->GetOriSize();
	} else {
		return m_size.Size();
	}
}

void ImageSymbol::InitTex(Texture* tex, const sm::i16_rect& region)
{
	cu::RefCountObjAssign(m_tex, tex);
	m_region = region;
}

sm::rect ImageSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return m_size;
}

void ImageSymbol::DrawBlend(const RenderParams& rp, sm::vec2* vertices, float* texcoords, int tex_id) const
{
	if (!m_tex) {
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());

	for (int i = 0; i < 4; ++i) {
		vertices[i] += rp.vertex_offset;
	}

	sm::vec2 vertices_scr[4];
	vertices_scr[0] = rp.mt * sm::vec2(m_size.xmin, m_size.ymin);
	vertices_scr[1] = rp.mt * sm::vec2(m_size.xmax, m_size.ymin);
	vertices_scr[2] = rp.mt * sm::vec2(m_size.xmax, m_size.ymax);
	vertices_scr[3] = rp.mt * sm::vec2(m_size.xmin, m_size.ymax);

	const Camera* cam = Blackboard::Instance()->GetCamera();
	const OrthoCamera* ocam = NULL;
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

	shader->Draw(&vertices[0].x, texcoords, &tex_coords_base[0].x, tex_id, screen_cache_texid);
}

void ImageSymbol::DrawOrtho(const RenderParams& rp, sm::vec2* vertices, float* texcoords, int tex_id) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
// 	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
// 	shader->SetColor(trans.color);
	if (mgr->GetShaderType() == sl::FILTER) {
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
		shader->Draw(&vertices[0].x, texcoords, tex_id);
	} else if (mgr->GetShaderType() == sl::SPRITE2) {
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
		shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());
		shader->DrawQuad(&vertices[0].x, texcoords, tex_id);
	}
}

void ImageSymbol::DrawPseudo3D(const RenderParams& rp, sm::vec2* vertices, float* texcoords, int tex_id) const
{
	const Camera* cam = Blackboard::Instance()->GetCamera();
	assert(cam && cam->Type() == CAM_PSEUDO3D);
	const Pseudo3DCamera* pcam = static_cast<const Pseudo3DCamera*>(cam);

	float z[4];
	rp.camera.CalculateZ(pcam->GetAngle(), vertices, z);

	std::vector<sm::vec3> _vertices;
	_vertices.push_back(sm::vec3(vertices[0].x, vertices[0].y, z[0]));
	_vertices.push_back(sm::vec3(vertices[1].x, vertices[1].y, z[1]));
	_vertices.push_back(sm::vec3(vertices[2].x, vertices[2].y, z[2]));
	_vertices.push_back(sm::vec3(vertices[0].x, vertices[0].y, z[0]));
	_vertices.push_back(sm::vec3(vertices[2].x, vertices[2].y, z[2]));
	_vertices.push_back(sm::vec3(vertices[3].x, vertices[3].y, z[3]));

	std::vector<sm::vec2> _texcoords;
	_texcoords.push_back(sm::vec2(texcoords[0], texcoords[1]));
	_texcoords.push_back(sm::vec2(texcoords[2], texcoords[3]));
	_texcoords.push_back(sm::vec2(texcoords[4], texcoords[5]));
	_texcoords.push_back(sm::vec2(texcoords[0], texcoords[1]));
	_texcoords.push_back(sm::vec2(texcoords[4], texcoords[5]));
	_texcoords.push_back(sm::vec2(texcoords[6], texcoords[7]));

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE3);
	sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(mgr->GetShader(sl::SPRITE3));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(), rp.color.GetGMapABGR(), rp.color.GetBMapABGR());
	shader->Draw(&_vertices[0].x, &_texcoords[0].x, tex_id);
}

}