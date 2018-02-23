#ifndef _SPRITE2_DRAW_GAUSSIAN_BLUR_H_
#define _SPRITE2_DRAW_GAUSSIAN_BLUR_H_

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"

#include S2_MAT_HEADER
#include <painting2/RenderReturn.h>

namespace cooking { class DisplayList; }
namespace pt2 { class RenderColorCommon; class RenderTarget; }

namespace s2
{

class Sprite;
class RenderParams;

class DrawGaussianBlur
{
public:
	static pt2::RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp, int iterations);

	static pt2::RenderReturn DrawBlurToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, const Sprite* spr, const RenderParams& rp, int iterations);
	static pt2::RenderReturn DrawFromRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, const sm::vec2& offset);

private:
	static pt2::RenderReturn DrawInit(pt2::RenderTarget* rt, const Sprite* spr, const RenderParams& rp);

	static pt2::RenderReturn DrawBetweenRT(pt2::RenderTarget* src, pt2::RenderTarget* dst, bool hori, const pt2::RenderColorCommon& col, float tex_size);

}; // DrawGaussianBlur

}

#endif // _SPRITE2_DRAW_GAUSSIAN_BLUR_H_