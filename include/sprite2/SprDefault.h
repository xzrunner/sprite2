#ifndef _SPRITE2_SPR_DEFAULT_H_
#define _SPRITE2_SPR_DEFAULT_H_

#include <cu/cu_macro.h>

#include <memory>

namespace s2
{

class CompTransform;
class CompColorCommon;
class CompColorMap;
class CompCamera;
class CompShader;

class SprDefault
{
public:
	const CompTransform& Transform() const { return *m_transform; }

	const CompColorCommon& Color() const    { return *m_color_common; }
	const CompColorMap&    ColorMap() const { return *m_color_map; }

	const CompCamera& Camera() { return *m_camera; }

	const CompShader& Shader() { return *m_shader; }

private:
	std::unique_ptr<CompTransform>   m_transform;

	std::unique_ptr<CompColorCommon> m_color_common;
	std::unique_ptr<CompColorMap>    m_color_map;

	std::unique_ptr<CompCamera>      m_camera;

	std::unique_ptr<CompShader>      m_shader;

	CU_SINGLETON_DECLARATION(SprDefault)

}; // SprDefault

}

#endif // _SPRITE2_SPR_DEFAULT_H_