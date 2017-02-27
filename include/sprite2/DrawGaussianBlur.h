#ifndef _SPRITE2_DRAW_GAUSSIAN_BLUR_H_
#define _SPRITE2_DRAW_GAUSSIAN_BLUR_H_

#include "pre_defined.h"

#include S2_MAT_HEADER

namespace s2
{

class Sprite;
class RenderParams;
class RenderColor;
class RenderTarget;

class DrawGaussianBlur
{
public:
	static void Draw(const Sprite* spr, const RenderParams& rp, int iterations);

	static void DrawBlurToRT(RenderTarget* rt, const Sprite* spr, const RenderParams& rp, int iterations);
	static void DrawFromRT(RenderTarget* rt, const sm::vec2& offset);

private:
	static void DrawInit(RenderTarget* rt, const Sprite* spr, const RenderParams& rp);

	static void DrawBetweenRT(RenderTarget* src, RenderTarget* dst, bool hori, const RenderColor& col, float tex_size);

}; // DrawGaussianBlur

}

#endif // _SPRITE2_DRAW_GAUSSIAN_BLUR_H_