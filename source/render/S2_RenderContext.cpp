#include "S2_RenderContext.h"

#include <unirender/UR_RenderContext.h>
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
	, m_vp_w(0)
	, m_vp_h(0)
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

	m_proj_width  = width;
	m_proj_height = height;

	UpdateProjection();
}

void RenderContext::SetScreen(int width, int height)
{
	if (m_screen_width == width && m_screen_height == height ||
		m_vp_w != 0 && width != m_vp_w ||
		m_vp_h != 0 && height != m_vp_h) {
		return;
	}

	m_screen_width  = width;
	m_screen_height = height;

	UpdateViewport();
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

	m_proj_width = w;
	m_proj_height = h;
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
	sl::SubjectMVP2::Instance()->NotifyProjection(m_proj_width, m_proj_height);
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