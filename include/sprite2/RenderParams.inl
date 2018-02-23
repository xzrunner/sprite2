#ifndef _SPRITE2_RENDER_PARAMS_INL_
#define _SPRITE2_RENDER_PARAMS_INL_

namespace s2
{

static RenderParams DEFAULT_RP;

inline
RenderParams::RenderParams()
	: vertex_offset(0, 0)
	, actor(nullptr)
	, min_edge(0)
	, m_flags(0)
#ifndef S2_DISABLE_STATISTICS
	, level(0)
	, parent_id(0)
#endif // S2_DISABLE_STATISTICS
{
	SetChangeShader(true);
}

inline
RenderParams::RenderParams(const RenderParams& params)
{
	memcpy(this, &params, sizeof(params));
}

inline
RenderParams& RenderParams::operator = (const RenderParams& params)
{
	memcpy(this, &params, sizeof(params));
	return *this;
}

inline
void RenderParams::Reset()
{
	memcpy(this, &DEFAULT_RP, sizeof(DEFAULT_RP));
}

inline
void RenderParams::ResetRender()
{
#ifdef S2_FILTER_FULL
	render_filter = nullptr;
#else
	render_filter = pt2::FM_NULL;
#endif // S2_FILTER_FULL
	render_blend = pt2::BM_NULL;
	render_fast_blend = pt2::FBM_NULL;
	render_downsample = 1;
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

}

#endif // _SPRITE2_RENDER_PARAMS_INL_
