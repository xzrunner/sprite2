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
	virtual pt2::RenderReturn DrawSpr2RT(cooking::DisplayList* dlist, const Sprite* spr, 
		const RenderParams& rp, bool too_large) const override;

	virtual pt2::RenderReturn DrawRT2ScreenSmall(cooking::DisplayList* dlist, int tex_id, 
		const Sprite* spr, const RenderParams& rp, bool reset_color) const override;

}; // DrawBlend

}

#endif // _SPRITE2_DRAW_BLEND_H_