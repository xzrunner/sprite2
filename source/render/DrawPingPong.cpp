#include "DrawPingPong.h"
#include "RenderCtxStack.h"
#include "S2_RenderTargetMgr.h"
#include "RenderScissor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "S2_RenderTarget.h"
#include "sprite2/Utility.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

DrawPingPong::DrawPingPong(int stat_pp_type)
	: m_stat_pp_type(stat_pp_type)
{
}

RenderReturn DrawPingPong::Draw(const Sprite* spr, const RenderParams& rp) const
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	sm::rect sz;
	spr->GetBounding(rp.actor)->CombineTo(sz);
	const bool too_large = sz.Width() > RT->WIDTH || sz.Height() > RT->HEIGHT;

	RenderTarget* rt = too_large ? RT->FetchScreen() : RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::Type(m_stat_pp_type));
#endif // S2_DISABLE_STATISTICS

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	if (!too_large) {
		RenderCtxStack::Instance()->Push(RenderContext(
			static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));
	}

	rt->Bind();
	ret |= DrawSpr2RT(spr, rp, too_large);
	rt->Unbind();

	if (!too_large) {
		RenderCtxStack::Instance()->Pop();
	}
	RenderScissor::Instance()->Enable();

	ret |= DrawRT2Screen(rt->GetTexID(), spr, rp, too_large);
	if (too_large) {
		RT->ReturnScreen(rt);
	} else {
		RT->Return(rt);
	}

	return ret;
}

RenderReturn DrawPingPong::DrawRT2Screen(int tex_id, const Sprite* spr, 
										 const RenderParams& rp, bool too_large) const
{
	RenderReturn ret = RENDER_OK;
	if (too_large) {
		ret |= DrawRT2ScreenLarge(tex_id, spr, rp, true);
	} else {
		ret |= DrawRT2ScreenSmall(tex_id, spr, rp, true);
	}
	return ret;
}

RenderReturn DrawPingPong::DrawRT2ScreenSmall(int tex_id, const Sprite* spr, 
											  const RenderParams& rp, bool reset_color) const
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

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	if (reset_color) {
		shader->SetColor(0xffffffff, 0);
		shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
		shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
	} else {
		RenderColor* col = RenderColorPool::Instance()->Pop();
		Utility::PrepareColor(rp.color, spr, rp.actor, *col);
		shader->SetColor(col->GetMulABGR(), col->GetAddABGR());
		shader->SetColorMap(col->GetRMapABGR(),col->GetGMapABGR(), col->GetBMapABGR());
		shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
		RenderColorPool::Instance()->Push(col);
	}

	return RENDER_OK;
}

RenderReturn DrawPingPong::DrawRT2ScreenLarge(int tex_id, const Sprite* spr,
											  const RenderParams& rp, bool reset_color) const
{
	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(2, 2, 0, 0));

	float xmin = -1, ymin = -1;
	float xmax =  1, ymax =  1;
	float vertices[8], texcoords[8];
	vertices[0] = xmin; vertices[1] = ymin;
	vertices[2] = xmax; vertices[3] = ymin;
	vertices[4] = xmax; vertices[5] = ymax;
	vertices[6] = xmin; vertices[7] = ymax;
	texcoords[0] = 0; texcoords[1] = 0;
	texcoords[2] = 1; texcoords[3] = 0;
	texcoords[4] = 1; texcoords[5] = 1;
	texcoords[6] = 0; texcoords[7] = 1;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	if (reset_color) {
		shader->SetColor(0xffffffff, 0);
		shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
		shader->DrawQuad(vertices, texcoords, tex_id);
	} else {
		RenderColor* col = RenderColorPool::Instance()->Pop();
		Utility::PrepareColor(rp.color, spr, rp.actor, *col);
		shader->SetColor(col->GetMulABGR(), col->GetAddABGR());
		shader->SetColorMap(col->GetRMapABGR(),col->GetGMapABGR(), col->GetBMapABGR());
		shader->DrawQuad(vertices, texcoords, tex_id);
		RenderColorPool::Instance()->Push(col);
	}

	s2::RenderCtxStack::Instance()->Pop();

	return RENDER_OK;
}

}