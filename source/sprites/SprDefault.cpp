#include "sprite2/SprDefault.h"
#include "sprite2/CompTransform.h"
#include "sprite2/CompColorCommon.h"
#include "sprite2/CompColorMap.h"

#include "sprite2/SprRender.h"
#include "sprite2/RenderShader.h"
#include "sprite2/RenderCamera.h"

namespace s2
{

CU_SINGLETON_DEFINITION(SprDefault);

SprDefault::SprDefault()
{
	m_transform = std::make_unique<CompTransform>();

	m_color     = std::make_unique<CompColorCommon>();
	m_color_map = std::make_unique<CompColorMap>();

	m_shader = new RenderShader();
	m_camera = new RenderCamera();

	m_render = new SprRender(m_shader, m_camera);
}

}