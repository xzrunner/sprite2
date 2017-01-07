#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

//#include "Matrix.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"
#include "SprTreePath.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>

namespace s2
{

class Sprite;
class SprTreePath;

class RenderParams
{
public:
	sm::vec2 vertex_offset;		// for blend

	sm::rect view_region;

	sm::mat4 mt;

	RenderColor	color;
	
	RenderShader shader;

	RenderCamera camera;

	bool set_shader;

	bool disable_render;

	bool disable_blend;
	bool disable_filter;
	
	SprTreePath path;

public:
	RenderParams();

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_