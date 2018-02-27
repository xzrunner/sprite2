#include "sprite2/StaticQuadIcon.h"
#include "sprite2/RenderParams.h"
#include "sprite2/ImageSymbol.h"

#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/Texture.h>

#include <string.h>

namespace s2
{

StaticQuadIcon::StaticQuadIcon()
{
	memset(m_src, 0, sizeof(m_src));
	memset(m_screen, 0, sizeof(m_screen));
}

StaticQuadIcon::StaticQuadIcon(const StaticQuadIcon& icon)
	: Icon(icon)
{
	memset(m_src, 0, sizeof(m_src));
	memset(m_screen, 0, sizeof(m_screen));
}

StaticQuadIcon& StaticQuadIcon::operator = (const StaticQuadIcon& icon)
{
	Icon::operator = (icon);
	memset(m_src, 0, sizeof(m_src));
	memset(m_screen, 0, sizeof(m_screen));
	return *this;
}

pt2::RenderReturn StaticQuadIcon::Draw(cooking::DisplayList* dlist, const RenderParams& rp, float process) const
{
	if (!m_img) {
		return pt2::RENDER_NO_DATA;
	}

	sm::vec2 vertices[4];
	for (int i = 0; i < 4; ++i) {
		vertices[i] = rp.mt * m_screen[i];
	}

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
	shader->DrawQuad(&vertices[0].x, &m_src[0].x, m_img->GetTexture()->GetTexID());

	return pt2::RENDER_OK;
}

sm::rect StaticQuadIcon::GetRegion(float process) const
{
	sm::rect ret;
	for (int i = 0; i < 4; ++i) {
		float x = m_screen[i].x,
			  y = m_screen[i].y;
		if (x < ret.xmin) ret.xmin = x;
		if (x > ret.xmax) ret.xmax = x;
		if (y < ret.ymin) ret.ymin = y;
		if (y > ret.ymax) ret.ymax = y;
	}
	return ret;
}

void StaticQuadIcon::SetScreen(const sm::vec2* screen)
{
	sm::vec2 sz = m_img->GetNoTrimedSize();
	memcpy(m_screen, screen, sizeof(m_screen));
	for (int i = 0; i < 4; ++i) {
		m_src[i].x = m_screen[i].x / sz.x + 0.5f;
		m_src[i].y = m_screen[i].y / sz.y + 0.5f;
	}
}

void StaticQuadIcon::GetQuad(float process, sm::vec2 quad[4]) const
{
	memcpy(&quad[0], &m_src[0], sizeof(m_src));
}

void StaticQuadIcon::Update()
{
	m_src[0].Set(0, 0);
	m_src[1].Set(0, 1);
	m_src[2].Set(1, 1);
	m_src[3].Set(1, 0);

	sm::vec2 sz = m_img->GetNoTrimedSize();
	float hw = sz.x * 0.5f,
		  hh = sz.y * 0.5f;
	m_screen[0].Set(-hw, -hh);
	m_screen[1].Set(-hw,  hh);
	m_screen[2].Set( hw,  hh);
	m_screen[3].Set( hw, -hh);	
}

}