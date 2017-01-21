#ifndef _SPRITE2_DRAW_BLEND_H_
#define _SPRITE2_DRAW_BLEND_H_

#include "pre_defined.h"

#include S2_MAT_HEADER

namespace s2
{

class Sprite;

class DrawBlend
{
public:
	static void Draw(const Sprite* spr, const S2_MAT& mt);

private:
	static void DrawSpr2RT(const Sprite* spr, const S2_MAT& mt);
	static void DrawRT2Screen(int tex_id, const Sprite* spr, const S2_MAT& mt);

}; // DrawBlend

}

#endif // _SPRITE2_DRAW_BLEND_H_