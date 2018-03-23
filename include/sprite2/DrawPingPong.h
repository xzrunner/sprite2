#ifndef _SPRITE2_DRAW_PINGPONG_H_
#define _SPRITE2_DRAW_PINGPONG_H_

#include "sprite2/typedef.h"
#include "sprite2/config.h"

#include <painting2/RenderReturn.h>

namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class RenderParams;

class DrawPingPong
{
public:
#ifndef S2_DISABLE_STATISTICS
	DrawPingPong(int stat_pp_type);
#else
	DrawPingPong();
#endif // S2_DISABLE_STATISTICS

	pt2::RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp) const;

protected:
	virtual pt2::RenderReturn DrawSpr2RT(cooking::DisplayList* dlist, const Sprite* spr, 
		const RenderParams& rp, bool too_large) const = 0;
	virtual pt2::RenderReturn DrawRT2Screen(cooking::DisplayList* dlist, int tex_id, 
		const Sprite* spr, const RenderParams& rp, bool too_large) const;

	virtual pt2::RenderReturn DrawRT2ScreenSmall(cooking::DisplayList* dlist, int tex_id, 
		const Sprite* spr, const RenderParams& rp, bool reset_color) const;
	virtual pt2::RenderReturn DrawRT2ScreenLarge(cooking::DisplayList* dlist, int tex_id, const Sprite* spr,
		const RenderParams& rp, bool reset_color) const;

private:
	int m_stat_pp_type;

}; // DrawUtility

}

#endif // _SPRITE2_DRAW_PINGPONG_H_