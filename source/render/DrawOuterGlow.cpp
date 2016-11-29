#include "DrawOuterGlow.h"
#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"
#include "SprDefault.h"
#include "SprRender.h"

#include <shaderlab.h>

namespace s2
{

void DrawOuterGlow::Draw(const Sprite* spr, const RenderParams& params, int iterations)
{
	DrawGaussianBlur::DrawToFbo0(spr, params, iterations);
	DrawGaussianBlur::DrawToScreen(true, spr->GetPosition());

	RenderParams _params = params;
	_params.set_shader = false;
	_params.disable_render = true;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	DrawNode::Draw(spr, _params);
}

}