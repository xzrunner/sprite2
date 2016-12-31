#ifndef _SPRITE2_DRAW_BLEND_H_
#define _SPRITE2_DRAW_BLEND_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;

class DrawBlend
{
public:
	static void Draw(const Sprite* spr, const sm::mat4& mt);

private:
	static void DrawSpr2RT(int rt, const Sprite* spr, const sm::mat4& mt);
	static void DrawRT2Screen(int rt, const Sprite* spr, const sm::mat4& mt);

}; // DrawBlend

}

#endif // _SPRITE2_DRAW_BLEND_H_