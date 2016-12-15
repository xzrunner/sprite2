#ifndef _SPRITE2_RENDER_PARAMS_INL_
#define _SPRITE2_RENDER_PARAMS_INL_

namespace s2
{

inline
RenderParams::RenderParams()
	: vertex_offset(0, 0)
	, set_shader(true)
	, disable_render(false)
	, disable_blend(false)
	, disable_filter(false)
{
}

}

#endif // _SPRITE2_RENDER_PARAMS_INL_