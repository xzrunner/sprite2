#ifndef _SPRITE2_P3D_RENDER_PARAMS_H_
#define _SPRITE2_P3D_RENDER_PARAMS_H_

#include "pre_defined.h"

#include S2_MAT_HEADER
#include "RenderColor.h"

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

public:
	P3dRenderParams() : local(true) {}

}; // P3dRenderParams

}

#endif // _SPRITE2_P3D_RENDER_PARAMS_H_