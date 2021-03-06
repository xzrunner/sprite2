#include "sprite2/DrawIntegrate.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"

#include <memmgr/Allocator.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <stat/StatPingPong.h>

namespace s2
{

DrawIntegrate::DrawIntegrate()
#ifndef S2_DISABLE_STATISTICS
	: DrawPingPong(st::StatPingPong::INTEGRATE)
#endif // S2_DISABLE_STATISTICS
{	
}

pt2::RenderReturn DrawIntegrate::DrawSpr2RT(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp, bool too_large) const
{
	ur::Blackboard::Instance()->GetRenderContext().Clear(0);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->col_common.Reset();
	rp_child->col_map.Reset();
	rp_child->ResetRender();
	rp_child->mt.Identity();
	rp_child->ClearViewRegion();

//	spr->SetMatDisable(true);
	spr->SetColorDisable(true);
//	spr->GetSymbol()->Draw(*rp_child, spr);
	rp_child->SetDisableIntegrate(true);
	DrawNode::Draw(nullptr, spr, *rp_child);
	spr->SetColorDisable(false);
//	spr->SetMatDisable(false);

	sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr().FlushShader();

	return pt2::RENDER_OK;
}

pt2::RenderReturn DrawIntegrate::DrawRT2Screen(cooking::DisplayList* dlist, int tex_id, const Sprite* spr,
										  const RenderParams& rp, bool too_large) const
{
	pt2::RenderReturn ret = pt2::RENDER_OK;
	if (too_large) {
		ret |= DrawRT2ScreenLarge(dlist, tex_id, spr, rp, false);
	} else {
		ret |= DrawRT2ScreenSmall(dlist, tex_id, spr, rp, false);
	}
	return ret;
}

}