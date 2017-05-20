#include "SprRender.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

namespace s2
{

SprRender::SprRender()
{
	Init();

	//////////////////////////////////////////////////////////////////////////

//	m_state.shader.filter = FilterFactory::Instance()->Create(FM_NULL);

// 	FilterMode fm = FM_NULL;
// 	if (spr.Shader().filter) {
// 		fm = spr.Shader().filter->GetMode();
// 	}
// 	m_state.shader.filter = FilterFactory::Instance()->Create(fm);
}

SprRender::SprRender(RenderColor* color, RenderShader* shader, 
					 RenderCamera* camera, float downsample)
{
	m_state.color = color;
	m_state.shader = shader;
	m_state.camera = camera;
	m_state.downsample = downsample;
}

SprRender::~SprRender()
{
	Term();
}

void SprRender::SetColor(const RenderColor& color)
{
	if (m_state.color) {
		RenderColorPool::Instance()->Push(m_state.color);
	}
	m_state.color = RenderColorPool::Instance()->Pop();
	*m_state.color = color;
}

void SprRender::SetShader(const RenderShader& shader)
{
	if (m_state.shader) {
		RenderShaderPool::Instance()->Push(m_state.shader);
	}
	m_state.shader = RenderShaderPool::Instance()->Pop();
	*m_state.shader = shader;
}

void SprRender::SetCamera(const RenderCamera& camera)
{
	if (m_state.camera) {
		RenderCameraPool::Instance()->Push(m_state.camera);
	}
	m_state.camera = RenderCameraPool::Instance()->Pop();
	*m_state.camera = camera;
}

void SprRender::SetDownsample(float downsample)
{
	m_state.downsample = downsample;
}

void SprRender::Term()
{
	if (m_state.color) {
		RenderColorPool::Instance()->Push(m_state.color);
		m_state.color  = NULL;
	}
	if (m_state.shader) {
		RenderShaderPool::Instance()->Push(m_state.shader);
		m_state.shader = NULL;
	}
	if (m_state.camera) {
		RenderCameraPool::Instance()->Push(m_state.camera);
		m_state.camera = NULL;
	}
}

void SprRender::Init()
{
	m_state.color  = NULL;
	m_state.shader = NULL;
	m_state.camera = NULL;
	m_state.downsample = 1;
}

}