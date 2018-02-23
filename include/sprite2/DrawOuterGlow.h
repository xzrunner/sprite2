#ifndef _SPRITE2_DRAW_OUTER_GLOW_H_
#define _SPRITE2_DRAW_OUTER_GLOW_H_

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"

#include S2_MAT_HEADER
#include <painting2/RenderReturn.h>

namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class RenderParams;

class DrawOuterGlow
{
public:
	static pt2::RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp, int iterations);

}; // DrawOuterGlow

}

#endif // _SPRITE2_DRAW_OUTER_GLOW_H_