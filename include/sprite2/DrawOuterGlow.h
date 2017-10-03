#ifndef _SPRITE2_DRAW_OUTER_GLOW_H_
#define _SPRITE2_DRAW_OUTER_GLOW_H_

#include "pre_defined.h"
#include "RenderReturn.h"

#include S2_MAT_HEADER

#include "s2_typedef.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawOuterGlow
{
public:
	static RenderReturn Draw(const SprConstPtr& spr, const RenderParams& rp, int iterations);

}; // DrawOuterGlow

}

#endif // _SPRITE2_DRAW_OUTER_GLOW_H_