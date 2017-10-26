#include "sprite2/SprDefault.h"
#include "sprite2/SprGeo.h"
#include "sprite2/SprRender.h"
#include "sprite2/RenderColor.h"
#include "sprite2/RenderShader.h"
#include "sprite2/RenderCamera.h"

namespace s2
{

CU_SINGLETON_DEFINITION(SprDefault);

SprDefault::SprDefault()
{
	m_geo    = new SprGeo();

	m_color  = new RenderColor();
	m_shader = new RenderShader();
	m_camera = new RenderCamera();

	m_render = new SprRender(m_color, m_shader, m_camera);
}

}