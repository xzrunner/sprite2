#ifndef _SPRITE2_RENDER_PARAMS_INL_
#define _SPRITE2_RENDER_PARAMS_INL_

namespace s2
{

//static RenderParams IDENTITY;

inline
RenderParams::RenderParams()
	: vertex_offset(0, 0)
	, actor(NULL)
	, m_flags(0)
	, m_next(NULL)
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
	, m_next(NULL)
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

inline
void RenderParams::Reset()
{
	vertex_offset.Set(0, 0);

	view_region.MakeEmpty();

	mt.Identity();

	color.Init();
	shader.Init();
	camera.Init();

	actor = NULL;

	m_flags = 0;

	SetChangeShader(true);
}

}

#endif // _SPRITE2_RENDER_PARAMS_INL_