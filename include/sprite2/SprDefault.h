#ifndef _SPRITE2_SPR_DEFAULT_H_
#define _SPRITE2_SPR_DEFAULT_H_

#include <CU_Singleton.h>

namespace s2
{

class SprGeo;
class SprRender;
class RenderColor;
class RenderShader;
class RenderCamera;

class SprDefault
{
public:
	SprGeo* Geo() { return m_geo; }

	SprRender* Render() { return m_render; }

	RenderColor* Color() { return m_color; }
	RenderShader* Shader() { return m_shader; }
	RenderCamera* Camera() { return m_camera; }

private:
	SprGeo*       m_geo;

	SprRender*    m_render;
	
	RenderColor*  m_color;
	RenderShader* m_shader;
	RenderCamera* m_camera;

	SINGLETON_DECLARATION(SprDefault)

}; // SprDefault

}

#endif // _SPRITE2_SPR_DEFAULT_H_