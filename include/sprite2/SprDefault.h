#ifndef _SPRITE2_SPR_DEFAULT_H_
#define _SPRITE2_SPR_DEFAULT_H_

#include <cu/cu_macro.h>

#include <memory>

namespace s2
{

class CompTransform;
class SprRender;
class RenderColor;
class RenderShader;
class RenderCamera;

class SprDefault
{
public:
	const CompTransform& Transform() const { return *m_transform; }

	auto& Render() { return m_render; }

	auto& Color() { return m_color; }
	auto& Shader() { return m_shader; }
	auto& Camera() { return m_camera; }

private:
	std::unique_ptr<CompTransform> m_transform;

	SprRender*    m_render;
	
	RenderColor*  m_color;
	RenderShader* m_shader;
	RenderCamera* m_camera;

	CU_SINGLETON_DECLARATION(SprDefault)

}; // SprDefault

}

#endif // _SPRITE2_SPR_DEFAULT_H_