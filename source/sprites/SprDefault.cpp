#include "SprDefault.h"
#include "SprGeo.h"
#include "SprRender.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

namespace s2
{

SINGLETON_DEFINITION(SprDefault);

SprDefault::SprDefault()
{
	m_geo = new SprGeo;
	m_geo->m_offset.Set(0, 0);

	m_color  = new RenderColor;
	m_shader = new RenderShader;
	m_camera = new RenderCamera;

	m_render = new SprRender;
	m_render->m_color  = m_color;
	m_render->m_shader = m_shader;
	m_render->m_camera = m_camera;
}

}