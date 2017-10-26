#ifndef _SPRITE2_DRAW_BLEND_H_
#define _SPRITE2_DRAW_BLEND_H_

#include "sprite2/DrawPingPong.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawBlend : public DrawPingPong
{
public:
	DrawBlend();

protected:
	virtual RenderReturn DrawSpr2RT(const Sprite* spr, const RenderParams& rp, bool too_large) const;

	virtual RenderReturn DrawRT2ScreenSmall(int tex_id, const Sprite* spr, 
		const RenderParams& rp, bool reset_color) const;

}; // DrawBlend

}

#endif // _SPRITE2_DRAW_BLEND_H_