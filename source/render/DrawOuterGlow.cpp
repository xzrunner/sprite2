#include "DrawOuterGlow.h"
#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"
#include "SprDefault.h"
#include "SprRender.h"
#include "S2_RenderTargetMgr.h"
#include "sprite2/Statistics.h"

#include <shaderlab/ShaderMgr.h>

namespace s2
{

void DrawOuterGlow::Draw(const Sprite* spr, const RenderParams& rp, int iterations)
{
	Statistics::Instance()->AddOuterGlow();

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
	DrawNode::Draw(spr, *rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 
}

}