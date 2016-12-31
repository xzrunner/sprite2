#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;
class RenderParams;
class RenderColor;

class DrawMask
{
public:
	static void Draw(const Sprite* base, const Sprite* mask, const RenderParams& params);

private:
	static void DrawBaseToRT(int rt, const Sprite* base, const RenderColor& rc);
	static void DrawMaskToRT(int rt, const Sprite* mask);

	static void DrawMaskFromRT(int rt_base, int rt_mask, const Sprite* mask, const sm::mat4& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_