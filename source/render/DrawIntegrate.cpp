#include "DrawIntegrate.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "S2_RenderTarget.h"
#include "S2_RenderTargetMgr.h"
#include "RenderScissor.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS
#include "sprite2/Utility.h"

#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

RenderReturn DrawIntegrate::Draw(const Sprite* spr, const RenderParams& rp)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddDownSample();
#endif // S2_DISABLE_STATISTICS

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	rt->Bind();
	DrawSpr2RT(spr, rp);
	rt->Unbind();

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	DrawRT2Screen(rt->GetTexID(), spr, rp);

	RT->Return(rt);

	return RENDER_OK;
}

RenderReturn DrawIntegrate::DrawSpr2RT(const Sprite* spr, const RenderParams& rp)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	rp_child->color.Init();
	rp_child->shader.Init();
	rp_child->mt.Identity();
	rp_child->view_region.MakeEmpty();

	spr->SetMatDisable(true);
	spr->SetColorDisable(true);
	spr->GetSymbol()->Draw(*rp_child, spr);
	spr->SetColorDisable(false);
	spr->SetMatDisable(false);

	RenderParamsPool::Instance()->Push(rp_child); 

	mgr->FlushShader();

	return RENDER_OK;
}

RenderReturn DrawIntegrate::DrawRT2Screen(int tex_id, const Sprite* spr, const RenderParams& rp)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	S2_MAT t = spr->GetLocalMat() * rp.mt;
	sm::rect r = spr->GetSymbol()->GetBounding();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = t * vertices[i];
	}

	sm::vec2 texcoords[4];
	texcoords[0] = sm::vec2(r.xmin, r.ymin);
	texcoords[1] = sm::vec2(r.xmin, r.ymax);
	texcoords[2] = sm::vec2(r.xmax, r.ymax);
	texcoords[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

	RenderColor* col = RenderColorPool::Instance()->Pop();
	Utility::PrepareColor(rp.color, spr, rp.actor, *col);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(col->GetMulABGR(), col->GetAddABGR());
	shader->SetColorMap(col->GetRMapABGR(),col->GetGMapABGR(), col->GetBMapABGR());
	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);

	RenderColorPool::Instance()->Push(col);

	return RENDER_OK;
}

}