#ifndef _SPRITE2_DRAW_INTEGRATE_H_
#define _SPRITE2_DRAW_INTEGRATE_H_

#include "DrawPingPong.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawIntegrate : public DrawPingPong
{
public:
	DrawIntegrate();

protected:
	virtual RenderReturn DrawSpr2RT(const Sprite* spr, const RenderParams& rp, bool too_large) const;
	virtual RenderReturn DrawRT2Screen(int tex_id, const Sprite* spr, const RenderParams& rp, bool too_large) const;

}; // DrawIntegrate

}

#endif // _SPRITE2_DRAW_INTEGRATE_H_