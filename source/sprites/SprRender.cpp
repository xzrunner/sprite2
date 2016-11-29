#include "SprRender.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

namespace s2
{

SprRender::SprRender()
	: m_color(NULL)
	, m_shader(NULL)
	, m_camera(NULL)
{
//	m_shader.filter = FilterFactory::Instance()->Create(FM_NULL);

// 	FilterMode fm = FM_NULL;
// 	if (spr.Shader().filter) {
// 		fm = spr.Shader().filter->GetMode();
// 	}
// 	m_shader.filter = FilterFactory::Instance()->Create(fm);
}

SprRender::~SprRender()
{
	if (m_color) {
		delete m_color;
	}
	if (m_shader) {
		delete m_shader;
	}
	if (m_camera) {
		delete m_camera;
	}
}

}