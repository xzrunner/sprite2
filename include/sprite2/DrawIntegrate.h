#ifndef _SPRITE2_DRAW_INTEGRATE_H_
#define _SPRITE2_DRAW_INTEGRATE_H_

#include "RenderReturn.h"

namespace s2
{

class Sprite;
class RenderParams;

class DrawIntegrate
{
public:
	static RenderReturn Draw(const Sprite* spr, const RenderParams& rp);

private:
	static RenderReturn DrawSpr2RT(const Sprite* spr, const RenderParams& rp);
	static RenderReturn DrawRT2Screen(int tex_id, const Sprite* spr, const RenderParams& rp);

}; // DrawIntegrate

}

#endif // _SPRITE2_DRAW_INTEGRATE_H_