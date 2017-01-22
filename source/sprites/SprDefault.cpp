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
	m_geo    = new SprGeo();

	m_color  = new RenderColor();
	m_shader = new RenderShader();
	m_camera = new RenderCamera();

	m_render = new SprRender(m_color, m_shader, m_camera);
}

}