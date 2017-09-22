#include "RenderScreen.h"
#include "RenderCtxStack.h"

#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

void RenderScreen::Scissor(float x, float y, float w, float h)
{
	const RenderContext* ctx = RenderCtxStack::Instance()->Top();
	if (!ctx) {
		sl::ShaderMgr::Instance()->GetContext()->SetScissor(0, 0, 0, 0);
		return;
	}

	float mv_scale = ctx->GetMVScale();
	x *= mv_scale;
	y *= mv_scale;
	w *= mv_scale;
	h *= mv_scale;

	float screen_w = static_cast<float>(ctx->GetScreenWidth()),
		  screen_h = static_cast<float>(ctx->GetScreenHeight());
	x += screen_w * 0.5f;
	y += screen_h * 0.5f;

	x += ctx->GetMVOffset().x * mv_scale;
	y += ctx->GetMVOffset().y * mv_scale;

	if (x < 0) {
		w += x;
		x = 0;
	} else if (x > screen_w) {
		w = h = 0;
	}
	if (y < 0) {
		h += y;
		y = 0;
	} else if (y > screen_h) {
		w = h = 0;
	}

	if (w < 0 || h < 0) { 
		w = h = 0;
	}

	sl::ShaderMgr::Instance()->GetContext()->SetScissor(
		static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
}

}