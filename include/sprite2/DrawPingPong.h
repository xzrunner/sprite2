#ifndef _SPRITE2_DRAW_PINGPONG_H_
#define _SPRITE2_DRAW_PINGPONG_H_

#include "s2_typedef.h"

#include "RenderReturn.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawPingPong
{
public:
	DrawPingPong(int stat_pp_type);

	RenderReturn Draw(const SprConstPtr& spr, const RenderParams& rp) const;

protected:
	virtual RenderReturn DrawSpr2RT(const SprConstPtr& spr, const RenderParams& rp, bool too_large) const = 0;
	virtual RenderReturn DrawRT2Screen(int tex_id, const SprConstPtr& spr, const RenderParams& rp, bool too_large) const;

	virtual RenderReturn DrawRT2ScreenSmall(int tex_id, const SprConstPtr& spr, 
		const RenderParams& rp, bool reset_color) const;
	virtual RenderReturn DrawRT2ScreenLarge(int tex_id, const SprConstPtr& spr,
		const RenderParams& rp, bool reset_color) const;

private:
	int m_stat_pp_type;

}; // DrawUtility

}

#endif // _SPRITE2_DRAW_PINGPONG_H_