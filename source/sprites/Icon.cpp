#include "sprite2/Icon.h"
#include "sprite2/RenderParams.h"
#include "sprite2/ImageSymbol.h"
#include "sprite2/TexcoordsMap.h"

#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/Texture.h>

namespace s2
{

pt2::RenderReturn Icon::Draw(cooking::DisplayList* dlist, const RenderParams& rp, float process) const
{
	if (!m_img) {
		return pt2::RENDER_NO_DATA;
	}
	if (!m_img->GetTexture()->IsLoadFinished()) {
		return pt2::RENDER_NO_DATA;
	}

//	process = 0.5;

	// tex_id
	float _texcoords[8];
	int tex_id;
	if (!m_img->QueryTexcoords(!rp.IsDisableDTexC2(), _texcoords, tex_id)) {
		m_img->OnQueryTexcoordsFail(dlist);
	}

	// texcoords
	sm::vec2 texcoords[4];
	GenTexcoords(process, texcoords);

	// vertices
	sm::vec2 vertices[4];
	GenVertices(process, texcoords, vertices);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = rp.mt * vertices[i];
	}

	TexcoordsMap::Trans(_texcoords, texcoords);

	// draw
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);

	return pt2::RENDER_OK;
}

sm::rect Icon::GetRegion(float process) const
{
	sm::rect ret;
	if (!m_img) {
		return ret;
	}

	sm::vec2 texcoords[4];
	GenTexcoords(process, texcoords);

	sm::vec2 vertices[4];
	GenVertices(process, texcoords, vertices);
	for (int i = 0; i < 4; ++i) {
		float x = vertices[i].x,
			  y = vertices[i].y;
		if (x < ret.xmin) ret.xmin = x;
		if (x > ret.xmax) ret.xmax = x;
		if (y < ret.ymin) ret.ymin = y;
		if (y > ret.ymax) ret.ymax = y;
	}

	return ret;
}

void Icon::GenTexcoords(float process, sm::vec2* texcoords) const
{
	sm::vec2 quad[4];
	GetQuad(process, quad);
	for (int i = 0; i < 4; ++i) {
		texcoords[i].x = quad[i].x;
		texcoords[i].y = quad[i].y;
	}
}

void Icon::GenVertices(float process, const sm::vec2* texcoords, 
					   sm::vec2* vertices) const
{
	sm::vec2 sz = m_img->GetNoTrimedSize();
	for (int i = 0; i < 4; ++i) {
		vertices[i].x = (texcoords[i].x - 0.5f) * sz.x;
		vertices[i].y = (texcoords[i].y - 0.5f) * sz.y;
	}
}

void Icon::SetImage(const std::shared_ptr<ImageSymbol>& img)
{
	m_img = img;
	Update();
}

}