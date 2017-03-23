#ifndef _SPRITE2_RENDER_PARAMS_INL_
#define _SPRITE2_RENDER_PARAMS_INL_

namespace s2
{

inline
RenderParams::RenderParams()
	: vertex_offset(0, 0)
	, actor(NULL)
	, m_flags(0)
{
	SetChangeShader(true);
}

}

#endif // _SPRITE2_RENDER_PARAMS_INL_