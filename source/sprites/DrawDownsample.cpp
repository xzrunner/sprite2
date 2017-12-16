#include "sprite2/DrawDownsample.h"
#include "sprite2/RenderTarget.h"
#include "sprite2/RenderTargetMgr.h"
#include "sprite2/RenderScissor.h"
#include "sprite2/RenderCtxStack.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/SymType.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

RenderReturn DrawDownsample::Draw(cooking::DisplayList* dlist, const Sprite* spr, 
	                              const RenderParams& rp, float downsample)
{
	if (downsample <= 0) {
		return RENDER_NO_DATA;
	}

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::DOWN_SAMPLE);
#endif // S2_DISABLE_STATISTICS

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	rt->Bind();
	DrawSpr2RT(spr, rp, downsample);
	rt->Unbind();

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	DrawRT2Screen(dlist, rt->GetTexID(), spr, rp, downsample);

	RT->Return(rt);

	return RENDER_OK;
}

RenderReturn DrawDownsample::DrawSpr2RT(const Sprite* spr, const RenderParams& rp, float downsample)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->mt.Identity();
	rp_child->mt.Scale(downsample, downsample);
	rp_child->ClearViewRegion();

	spr->SetMatDisable(true);
	spr->GetSymbol()->DrawTree(nullptr, *rp_child, spr);
	spr->SetMatDisable(false);

	mgr->FlushShader();

	return RENDER_OK;
}

RenderReturn DrawDownsample::DrawRT2Screen(cooking::DisplayList* dlist, int tex_id, 
	                                       const Sprite* spr, const RenderParams& rp, float downsample)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	S2_MAT t = spr->GetLocalMat() * rp.mt;

	sm::rect r;
	if (spr->GetSymbol()->Type() == SYM_PARTICLE3D) {
// 		float hw = RT->WIDTH * downsample;
// 		float hh = RT->HEIGHT * downsample;
		float hw = RT->WIDTH * 0.5f;
		float hh = RT->HEIGHT * 0.5f;
		r.xmin = -hw;
		r.xmax =  hw;
		r.ymin = -hh;
		r.ymax =  hh;
	} else {
		r = spr->GetSymbol()->GetBounding();
	}

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
	S2_MAT mt;
	mt.Scale(downsample, downsample);
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = mt * texcoords[i];
	}
	for (int i = 0; i < 4; ++i) {
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));

	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
#else
	cooking::change_shader(dlist, sl::SPRITE2);
	cooking::set_color_sprite(dlist, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
	cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED

	return RENDER_OK;
}

}