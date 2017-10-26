#include "sprite2/DrawOuterGlow.h"
#include "sprite2/DrawGaussianBlur.h"
#include "sprite2/Sprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/FilterFactory.h"
#include "sprite2/DrawNode.h"
#include "sprite2/SprDefault.h"
#include "sprite2/SprRender.h"
#include "sprite2/RenderTargetMgr.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

RenderReturn DrawOuterGlow::Draw(const Sprite* spr, const RenderParams& rp, int iterations)
{
#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::OUTER_GLOW);
#endif // S2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	DrawGaussianBlur::DrawBlurToRT(rt, spr, rp, iterations);
	DrawGaussianBlur::DrawFromRT(rt, spr->GetPosition());
	RT->Return(rt);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->SetChangeShader(false);
	rp_child->SetDisableRenderDraw(true);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	RenderReturn ret = DrawNode::Draw(nullptr, spr, *rp_child);

	return ret;
}

}