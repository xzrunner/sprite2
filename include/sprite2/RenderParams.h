#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

//#include "Matrix.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <SM_Matrix.h>

namespace s2
{

class Sprite;

class RenderParams
{
public:
	sm::vec2 vertex_offset;		// for blend

	bool set_shader;

	sm::mat4 mt;

	RenderColor	color;
	
	RenderShader shader;

	RenderCamera camera;

	bool disable_render;

	bool disable_blend;
	bool disable_filter;
		
public:
	RenderParams();

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_