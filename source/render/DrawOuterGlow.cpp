#include "sprite2/DrawOuterGlow.h"
#include "sprite2/DrawGaussianBlur.h"
#include "sprite2/Sprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#include "sprite2/CompDefault.h"
#ifndef S2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/RenderTargetMgr.h>
#include <painting2/RenderTarget.h>

namespace s2
{

pt2::RenderReturn DrawOuterGlow::Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp, int iterations)
{
#ifndef S2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::OUTER_GLOW);
#endif // S2_DISABLE_STATISTICS

	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();
	pt2::RenderTarget* rt = RT->Fetch();
	DrawGaussianBlur::DrawBlurToRT(dlist, rt, spr, rp, iterations);
	DrawGaussianBlur::DrawFromRT(dlist, rt, spr->GetPosition());
	RT->Return(rt);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->SetChangeShader(false);
	rp_child->SetDisableRenderDraw(true);

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);
#else
	cooking::change_shader(nullptr, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED
	pt2::RenderReturn ret = DrawNode::Draw(nullptr, spr, *rp_child);

	return ret;
}

}