#include "sprite2/SprDefault.h"
#include "sprite2/CompTransform.h"
#include "sprite2/CompColorCommon.h"
#include "sprite2/CompColorMap.h"
#include "sprite2/CompCamera.h"
#include "sprite2/CompShader.h"

namespace s2
{

CU_SINGLETON_DEFINITION(SprDefault);

SprDefault::SprDefault()
{
	m_transform    = std::make_unique<CompTransform>();

	m_color_common = std::make_unique<CompColorCommon>();
	m_color_map    = std::make_unique<CompColorMap>();

	m_camera       = std::make_unique<CompCamera>();

	m_shader       = std::make_unique<CompShader>();
}

}