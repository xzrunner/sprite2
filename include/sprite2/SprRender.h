#ifndef _SPRITE2_SPR_RENDER_H_
#define _SPRITE2_SPR_RENDER_H_

#include <CU_RefCountObj.h>

namespace s2
{

class RenderColor;
class RenderShader;
class RenderCamera;

class SprRender : public cu::RefCountObj
{
public:
	SprRender();
	~SprRender();
	
private:
	RenderColor*  m_color;
	RenderShader* m_shader;
	RenderCamera* m_camera;

	friend class Sprite;
	friend class SprDefault;

}; // SprRender

}

#endif // _SPRITE2_SPR_RENDER_H_