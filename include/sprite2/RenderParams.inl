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

inline
RenderParams::RenderParams(const RenderParams& params)
	: vertex_offset(params.vertex_offset)
	, view_region(params.view_region)
	, mt(params.mt)
	, color(params.color)
	, shader(params.shader)
	, camera(params.camera)
	, actor(params.actor)
	, m_flags(params.m_flags)
{
}

inline
RenderParams& RenderParams::operator = (const RenderParams& params)
{
	vertex_offset = params.vertex_offset;
	view_region   = params.view_region;
	mt            = params.mt;
	color         = params.color;
	shader        = params.shader;
	camera        = params.camera;
	actor         = params.actor;
	m_flags       = params.m_flags;
	return *this;
}

}

#endif // _SPRITE2_RENDER_PARAMS_INL_