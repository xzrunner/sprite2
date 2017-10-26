#ifndef _SPRITE2_P3D_RENDER_PARAMS_H_
#define _SPRITE2_P3D_RENDER_PARAMS_H_

#include "sprite2/pre_defined.h"

#include S2_MAT_HEADER
#include "sprite2/RenderColor.h"

#include <SM_Rect.h>

namespace s2
{

class P3dRenderParams
{
public:
	S2_MAT      mt;
	RenderColor rc;
	bool        local;
	sm::rect    view_region;
	uint32_t    flags;

public:
	P3dRenderParams() : local(true), flags(0xffffffff) {}

}; // P3dRenderParams

}

#endif // _SPRITE2_P3D_RENDER_PARAMS_H_