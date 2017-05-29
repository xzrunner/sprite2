#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include "pre_defined.h"
#include "RenderReturn.h"

#include S2_MAT_HEADER

namespace s2
{

class Sprite;
class Actor;
class RenderParams;
class RenderColor;
class RenderTarget;

class DrawMask
{
public:
	static RenderReturn Draw(const Sprite* base, const Sprite* mask, const RenderParams& rp);

private:
	static RenderReturn DrawBaseToRT(RenderTarget* rt, const Sprite* base, const RenderColor& rc, const Actor* actor);
	static RenderReturn DrawMaskToRT(RenderTarget* rt, const Sprite* mask, const Actor* actor);

	static RenderReturn DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, const Sprite* mask, const S2_MAT& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_