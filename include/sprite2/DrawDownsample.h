#ifndef _SPRITE2_DRAW_DOWNSAMPLE_H_
#define _SPRITE2_DRAW_DOWNSAMPLE_H_

#include "RenderReturn.h"
#include "s2_typedef.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawDownsample
{
public:
	static RenderReturn Draw(const SprConstPtr& spr, const RenderParams& rp, float downsample);

private:
	static RenderReturn DrawSpr2RT(const SprConstPtr& spr, const RenderParams& rp, float downsample);
	static RenderReturn DrawRT2Screen(int tex_id, const SprConstPtr& spr, const RenderParams& rp, float downsample);

}; // DrawDownsample

}

#endif // _SPRITE2_DRAW_DOWNSAMPLE_H_