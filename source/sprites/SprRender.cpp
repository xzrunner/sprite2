#include "sprite2/SprRender.h"
#include "sprite2/RenderColor.h"
#include "sprite2/RenderShader.h"
#include "sprite2/RenderCamera.h"
#include "sprite2/SprDefault.h"

#include <memmgr/Allocator.h>

namespace s2
{

SprRender::SprRender()
	: m_color(SprDefault::Instance()->Color())
	, m_shader(SprDefault::Instance()->Shader())
	, m_camera(SprDefault::Instance()->Camera())
{
}

SprRender::SprRender(RenderColor* color, RenderShader* shader, RenderCamera* camera)
	: m_color(color)
	, m_shader(shader)
	, m_camera(camera)
{
}

SprRender::~SprRender()
{
	if (m_color != SprDefault::Instance()->Color()) {
		mm::AllocHelper::Delete(m_color);
	}
	if (m_shader != SprDefault::Instance()->Shader()) {
		mm::AllocHelper::Delete(m_shader);
	}
	if (m_camera != SprDefault::Instance()->Camera()) {
		mm::AllocHelper::Delete(m_camera);
	}
}

void SprRender::SetColor(const RenderColor& color)
{
	if (*m_color == color) {
		return;
	}
	if (m_color == SprDefault::Instance()->Color()) {
		m_color = mm::AllocHelper::New<RenderColor>(color);
	} else {
		*m_color = color;
	}
}

void SprRender::SetShader(const RenderShader& shader)
{
	if (*m_shader == shader) {
		return;
	}
	if (m_shader == SprDefault::Instance()->Shader()) {
		m_shader = mm::AllocHelper::New<RenderShader>(shader);
	} else {
		*m_shader = shader;
	}
}

void SprRender::SetCamera(const RenderCamera& camera)
{
	if (*m_camera == camera) {
		return;
	}
	if (m_camera == SprDefault::Instance()->Camera()) {
		m_camera = mm::AllocHelper::New<RenderCamera>(camera);
	} else {
		*m_camera = camera;
	}
}

}