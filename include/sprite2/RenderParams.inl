#ifndef _SPRITE2_RENDER_PARAMS_INL_
#define _SPRITE2_RENDER_PARAMS_INL_

namespace s2
{

//static RenderParams IDENTITY;

inline
RenderParams::RenderParams()
	: vertex_offset(0, 0)
	, actor(nullptr)
	, min_edge(0)
	, m_flags(0)
	, m_next(nullptr)
#ifndef S2_DISABLE_STATISTICS
	, level(0)
	, parent_id(0)
#endif // S2_DISABLE_STATISTICS
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
	, min_edge(params.min_edge)
	, m_flags(params.m_flags)
	, m_next(nullptr)
#ifndef S2_DISABLE_STATISTICS
	, level(params.level)
	, parent_id(params.parent_id)
#endif // S2_DISABLE_STATISTICS
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
	min_edge      = params.min_edge;
#ifndef S2_DISABLE_STATISTICS
	level         = params.level;
	parent_id     = params.parent_id;
#endif // S2_DISABLE_STATISTICS
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

	m_flags = 0;

	SetChangeShader(true);

	min_edge = 0;

#ifndef S2_DISABLE_STATISTICS
	level = 0;
	parent_id = 0;
#endif // S2_DISABLE_STATISTICS
}

inline
void RenderParams::SetViewRegion(float xmin, float ymin, float xmax, float ymax)
{
	view_region.xmin = xmin;
	view_region.ymin = ymin;
	view_region.xmax = xmax;
	view_region.ymax = ymax;
	SetViewRegionValid(view_region.IsValid());
}

inline
void RenderParams::SetViewRegion(const sm::rect& vr)
{
	view_region = vr;
	SetViewRegionValid(view_region.IsValid());
}

inline
void RenderParams::ClearViewRegion()
{
	view_region.MakeEmpty();
	SetViewRegionValid(false);
}

inline
void RenderParams::Init()
{
	Reset();
}

inline
void RenderParams::Term()
{
	color.Term();
	shader.Term();
	camera.Term();
}

}

#endif // _SPRITE2_RENDER_PARAMS_INL_