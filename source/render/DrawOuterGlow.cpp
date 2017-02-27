#include "DrawOuterGlow.h"
#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"
#include "SprDefault.h"
#include "SprRender.h"
#include "RenderTargetMgr.h"

#include <shaderlab/ShaderMgr.h>

namespace s2
{

void DrawOuterGlow::Draw(const Sprite* spr, const RenderParams& rp, int iterations)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	DrawGaussianBlur::DrawBlurToRT(rt, spr, rp, iterations);
	DrawGaussianBlur::DrawFromRT(rt, spr->GetPosition());
	RT->Return(rt);

	RenderParams rp_child = rp;
	rp_child.set_shader = false;
	rp_child.disable_render = true;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	DrawNode::Draw(spr, rp_child);
}

}