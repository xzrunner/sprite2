#ifndef _SPRITE2_SPR_DEFAULT_H_
#define _SPRITE2_SPR_DEFAULT_H_

#include <cu/cu_macro.h>

#include <memory>

namespace s2
{

class CompTransform;
class CompColorCommon;
class CompColorMap;

class SprRender;
class RenderShader;
class RenderCamera;

class SprDefault
{
public:
	const CompTransform& Transform() const { return *m_transform; }

	const CompColorCommon&    Color() const    { return *m_color; }
	const CompColorMap& ColorMap() const { return *m_color_map; }

	auto& Render() { return m_render; }

	auto& Shader() { return m_shader; }
	auto& Camera() { return m_camera; }

private:
	std::unique_ptr<CompTransform> m_transform;

	std::unique_ptr<CompColorCommon>     m_color;
	std::unique_ptr<CompColorMap>  m_color_map;

	SprRender*    m_render;
	
	RenderShader* m_shader;
	RenderCamera* m_camera;

	CU_SINGLETON_DECLARATION(SprDefault)

}; // SprDefault

}

#endif // _SPRITE2_SPR_DEFAULT_H_