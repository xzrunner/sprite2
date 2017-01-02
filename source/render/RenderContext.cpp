#include "RenderContext.h"

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
{}

RenderContext::RenderContext(float proj_width, float proj_height, int screen_width, int screen_height)
	: m_mv_offset(0, 0)
	, m_mv_scale(1)
	, m_proj_width(proj_width)
	, m_proj_height(proj_height) 
	, m_screen_width(screen_width)
	, m_screen_height(screen_height)
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
	if (m_screen_width == width && m_screen_height == height) {
		return;
	}

	m_screen_width  = width;
	m_screen_height = height;

	UpdateViewport();
}

void RenderContext::UpdateMVP() const
{
	UpdateModelView();
	UpdateProjection();
}

void RenderContext::UpdateViewport() const
{
	if (m_screen_width == 0 && m_screen_height == 0) {
		return;
	}

	sl::ShaderMgr::Instance()->GetContext()->SetViewport(
		0, 0, m_screen_width, m_screen_height);
}

void RenderContext::UpdateModelView() const
{
	sl::SubjectMVP2::Instance()->NotifyModelview(m_mv_offset.x, m_mv_offset.y, m_mv_scale, m_mv_scale);
}

void RenderContext::UpdateProjection() const
{
	sl::SubjectMVP2::Instance()->NotifyProjection(m_proj_width, m_proj_height);
}

}