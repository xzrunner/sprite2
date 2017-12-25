#ifndef _SPRITE2_P3D_RENDER_PARAMS_H_
#define _SPRITE2_P3D_RENDER_PARAMS_H_

#include "sprite2/pre_defined.h"

#include S2_MAT_HEADER
#include "sprite2/RenderColor.h"

#include <SM_Rect.h>

namespace s2
{

#ifdef S2_FILTER_FULL
class RenderFilter;
#endif // S2_FILTER_FULL

class P3dRenderParams
{
public:
	S2_MAT        mt;
	RenderColor   color;
#ifdef S2_FILTER_FULL
	RenderFilter* render_filter = nullptr;
#else
	FilterMode    render_filter = FM_NULL;
#endif // S2_FILTER_FULL
	bool          local = true;
	sm::rect      view_region;
	uint32_t      flags = 0xffffffff;

}; // P3dRenderParams

}

#endif // _SPRITE2_P3D_RENDER_PARAMS_H_