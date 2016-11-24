#ifndef _SPRITE2_RENDER_CONTEXT_H_
#define _SPRITE2_RENDER_CONTEXT_H_

#include <SM_Vector.h>

namespace s2
{

class RenderCtx
{
public:
	sm::vec2 mv_offset;
	float mv_scale;

	float proj_width;
	float proj_height;

	int screen_width;
	int screen_height;

public:
	RenderCtx() 
		: mv_scale(0), proj_width(0), proj_height(0) {}
	RenderCtx(float proj_width, float proj_height, int screen_width, int screen_height)
		: mv_offset(0, 0)
		, mv_scale(1)
		, proj_width(proj_width)
		, proj_height(proj_height) 
		, screen_width(screen_width)
		, screen_height(screen_height)
	{}

}; // RenderCtx

}

#endif // _SPRITE2_RENDER_CONTEXT_H_