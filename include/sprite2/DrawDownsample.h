#ifndef _SPRITE2_DRAW_DOWNSAMPLE_H_
#define _SPRITE2_DRAW_DOWNSAMPLE_H_

#include "sprite2/RenderReturn.h"
#include "sprite2/typedef.h"

namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class RenderParams;

class DrawDownsample
{
public:
	static RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp, float downsample);

private:
	static RenderReturn DrawSpr2RT(const Sprite* spr, const RenderParams& rp, float downsample);
	static RenderReturn DrawRT2Screen(cooking::DisplayList* dlist, int tex_id, const Sprite* spr, const RenderParams& rp, float downsample);

}; // DrawDownsample

}

#endif // _SPRITE2_DRAW_DOWNSAMPLE_H_