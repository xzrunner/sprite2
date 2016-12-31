#ifndef _SPRITE2_DRAW_GAUSSIAN_BLUR_H_
#define _SPRITE2_DRAW_GAUSSIAN_BLUR_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;
class RenderParams;
class RenderColor;

class DrawGaussianBlur
{
public:
	static void Draw(const Sprite* spr, const RenderParams& params, int iterations);

	static void DrawBlurToRT(int rt, const Sprite* spr, const RenderParams& params, int iterations);
	static void DrawFromRT(int rt, const sm::vec2& offset);

private:
	static void DrawInit(int rt, const Sprite* spr, const RenderParams& params);

	static void DrawBetweenRT(int src_rt, int dst_rt, bool hori, const RenderColor& col, float tex_size);

}; // DrawGaussianBlur

}

#endif // _SPRITE2_DRAW_GAUSSIAN_BLUR_H_