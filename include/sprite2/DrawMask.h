#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include "pre_defined.h"

#include S2_MAT_HEADER

namespace s2
{

class Sprite;
class RenderParams;
class RenderColor;
class RenderTarget;

class DrawMask
{
public:
	static void Draw(const Sprite* base, const Sprite* mask, const RenderParams& params);

private:
	static void DrawBaseToRT(RenderTarget* rt, const Sprite* base, const RenderColor& rc);
	static void DrawMaskToRT(RenderTarget* rt, const Sprite* mask);

	static void DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, const Sprite* mask, const S2_MAT& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_