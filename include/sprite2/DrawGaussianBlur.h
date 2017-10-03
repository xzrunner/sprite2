#ifndef _SPRITE2_DRAW_GAUSSIAN_BLUR_H_
#define _SPRITE2_DRAW_GAUSSIAN_BLUR_H_

#include "pre_defined.h"
#include "s2_typedef.h"
#include "RenderReturn.h"

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
	static RenderReturn Draw(const SprConstPtr& spr, const RenderParams& rp, int iterations);

	static RenderReturn DrawBlurToRT(RenderTarget* rt, const SprConstPtr& spr, const RenderParams& rp, int iterations);
	static RenderReturn DrawFromRT(RenderTarget* rt, const sm::vec2& offset);

private:
	static RenderReturn DrawInit(RenderTarget* rt, const SprConstPtr& spr, const RenderParams& rp);

	static RenderReturn DrawBetweenRT(RenderTarget* src, RenderTarget* dst, bool hori, const RenderColor& col, float tex_size);

}; // DrawGaussianBlur

}

#endif // _SPRITE2_DRAW_GAUSSIAN_BLUR_H_