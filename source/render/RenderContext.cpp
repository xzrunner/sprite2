#include "sprite2/RenderContext.h"

#include <unirender/RenderContext.h>
#include <shaderlab/SubjectMVP2.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

RenderContext::RenderContext() 
	: m_mv_scale(0)
	, m_proj_width(0)
	, m_proj_height(0)
	, m_screen_width(0)
	, m_screen_height(0)
	, m_vp_x(0)
	, m_vp_y(0) 
	, m_vp_w(0)
	, m_vp_h(0)
{}

RenderContext::RenderContext(float proj_width, float proj_height, int screen_width, int screen_height)
	: m_mv_offset(0, 0)
	, m_mv_scale(1)
	, m_proj_width(proj_width)
	, m_proj_height(proj_height) 
	, m_screen_width(screen_width)
	, m_screen_height(screen_height)
	, m_vp_x(0)
	, m_vp_y(0) 
	, m_vp_w(screen_width)
	, m_vp_h(screen_height)
{}

void RenderContext::SetModelView(const sm::vec2& offset, float scale)
{
	if (offset == m_mv_offset && scale == m_mv_scale) {
		return;
	} 

	m_mv_offset = offset;
	m_mv_scale  = scale;

	UpdateModelView();
}

void RenderContext::SetProjection(int width, int height)
{
	if (m_proj_width == width && m_proj_height == height) {
		return;
	}

	m_proj_width  = static_cast<float>(width);
	m_proj_height = static_cast<float>(height);

	UpdateProjection();
}

void RenderContext::SetScreen(int width, int height)
{
	m_screen_width  = width;
	m_screen_height = height;
}

void RenderContext::SetViewport(int x, int y, int w, int h)
{
	if (m_vp_x == x &&
		m_vp_y == y &&
		m_vp_w == w &&
		m_vp_h == h) {
		return;
	}

	m_vp_x = x;
	m_vp_y = y;
	m_vp_w = w;
	m_vp_h = h;
	UpdateViewport();

	m_proj_width  = static_cast<float>(w);
	m_proj_height = static_cast<float>(h);
	UpdateProjection();
}

void RenderContext::UpdateMVP() const
{
	UpdateModelView();
	UpdateProjection();
}

void RenderContext::UpdateModelView() const
{
	sl::SubjectMVP2::Instance()->NotifyModelview(m_mv_offset.x, m_mv_offset.y, m_mv_scale, m_mv_scale);
}

void RenderContext::UpdateProjection() const
{
	sl::SubjectMVP2::Instance()->NotifyProjection(static_cast<int>(m_proj_width), static_cast<int>(m_proj_height));
}

void RenderContext::UpdateViewport() const
{
	if (m_vp_w == 0 && m_vp_h == 0) {
		return;
	}

	sl::ShaderMgr::Instance()->GetContext()->SetViewport(
		m_vp_x, m_vp_y, m_vp_w, m_vp_h);
}

}