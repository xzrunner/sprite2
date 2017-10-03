#include "DrawOuterGlow.h"
#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"
#include "SprDefault.h"
#include "SprRender.h"
#include "S2_RenderTargetMgr.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS

#include <shaderlab/ShaderMgr.h>

namespace s2
{

RenderReturn DrawOuterGlow::Draw(const SprConstPtr& spr, const RenderParams& rp, int iterations)
{
#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::OUTER_GLOW);
#endif // S2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	DrawGaussianBlur::DrawBlurToRT(rt, spr, rp, iterations);
	DrawGaussianBlur::DrawFromRT(rt, spr->GetPosition());
	RT->Return(rt);

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	rp_child->SetChangeShader(false);
	rp_child->SetDisableRenderDraw(true);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	RenderReturn ret = DrawNode::Draw(spr, *rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

}