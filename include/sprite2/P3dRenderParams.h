#ifndef _SPRITE2_P3D_RENDER_PARAMS_H_
#define _SPRITE2_P3D_RENDER_PARAMS_H_

#include "sprite2/pre_defined.h"

#include S2_MAT_HEADER
#include <SM_Rect.h>
#include <painting2/RenderColorCommon.h>
#include <painting2/RenderColorMap.h>

namespace s2
{

#ifdef S2_FILTER_FULL
class RenderFilter;
#endif // S2_FILTER_FULL

class P3dRenderParams
{
public:
	S2_MAT        mt;
	pt2::RenderColorCommon col_common;
	pt2::RenderColorMap    col_map;
#ifdef S2_FILTER_FULL
	RenderFilter* render_filter = nullptr;
#else
	pt2::FilterMode    render_filter = pt2::FM_NULL;
#endif // S2_FILTER_FULL
	bool          local = true;
	sm::rect      view_region;
	uint32_t      flags = 0xffffffff;

}; // P3dRenderParams

}

#endif // _SPRITE2_P3D_RENDER_PARAMS_H_