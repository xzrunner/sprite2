#ifndef _SPRITE2_DRAW_OUTER_GLOW_H_
#define _SPRITE2_DRAW_OUTER_GLOW_H_

#include "sprite2/pre_defined.h"
#include "sprite2/RenderReturn.h"

#include S2_MAT_HEADER

#include "sprite2/typedef.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawOuterGlow
{
public:
	static RenderReturn Draw(const Sprite* spr, const RenderParams& rp, int iterations);

}; // DrawOuterGlow

}

#endif // _SPRITE2_DRAW_OUTER_GLOW_H_