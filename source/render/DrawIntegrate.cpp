#include "DrawIntegrate.h"
#include "sprite2/StatPingPong.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "DrawNode.h"

#include <memmgr/Allocator.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

DrawIntegrate::DrawIntegrate()
	: DrawPingPong(StatPingPong::INTEGRATE)
{	
}

RenderReturn DrawIntegrate::DrawSpr2RT(const Sprite* spr, const RenderParams& rp, bool too_large) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->color.Reset();
	rp_child->shader.Reset();
	rp_child->mt.Identity();
	rp_child->ClearViewRegion();

//	spr->SetMatDisable(true);
	spr->SetColorDisable(true);
//	spr->GetSymbol()->Draw(*rp_child, spr);
	rp_child->SetDisableIntegrate(true);
	DrawNode::Draw(spr, *rp_child);
	spr->SetColorDisable(false);
//	spr->SetMatDisable(false);

	mgr->FlushShader();

	return RENDER_OK;
}

RenderReturn DrawIntegrate::DrawRT2Screen(int tex_id, const Sprite* spr, 
										  const RenderParams& rp, bool too_large) const
{
	RenderReturn ret = RENDER_OK;
	if (too_large) {
		ret |= DrawRT2ScreenLarge(tex_id, spr, rp, false);
	} else {
		ret |= DrawRT2ScreenSmall(tex_id, spr, rp, false);
	}
	return ret;
}

}