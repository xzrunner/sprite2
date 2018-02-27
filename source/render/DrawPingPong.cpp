#include "sprite2/DrawPingPong.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Utility.h"
#ifndef S2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#endif // S2_DISABLE_STATISTICS

#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/RenderTargetMgr.h>
#include <painting2/RenderTarget.h>
#include <painting2/RenderCtxStack.h>
#include <painting2/RenderScissor.h>

namespace s2
{

DrawPingPong::DrawPingPong(int stat_pp_type)
	: m_stat_pp_type(stat_pp_type)
{
}

pt2::RenderReturn DrawPingPong::Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp) const
{
	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();

	sm::rect sz;
	spr->GetBounding(rp.actor).CombineTo(sz);
	const bool too_large = sz.Width() > RT->WIDTH || sz.Height() > RT->HEIGHT;

	pt2::RenderTarget* rt = too_large ? RT->FetchScreen() : RT->Fetch();
	if (!rt) {
		return pt2::RENDER_NO_RT;
	}

	pt2::RenderReturn ret = pt2::RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::Type(m_stat_pp_type));
#endif // S2_DISABLE_STATISTICS

	sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr().FlushShader();

	pt2::RenderScissor::Instance()->Disable();
	if (!too_large) {
		pt2::RenderCtxStack::Instance()->Push(pt2::RenderContext(
			static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));
	}

	rt->Bind();
	ret |= DrawSpr2RT(dlist, spr, rp, too_large);
	rt->Unbind();

	if (!too_large) {
		pt2::RenderCtxStack::Instance()->Pop();
	}
	pt2::RenderScissor::Instance()->Enable();

	ret |= DrawRT2Screen(dlist, rt->GetTexID(), spr, rp, too_large);
	if (too_large) {
		RT->ReturnScreen(rt);
	} else {
		RT->Return(rt);
	}

	return ret;
}

pt2::RenderReturn DrawPingPong::DrawRT2Screen(cooking::DisplayList* dlist, int tex_id, const Sprite* spr,
										 const RenderParams& rp, bool too_large) const
{
	pt2::RenderReturn ret = pt2::RENDER_OK;
	if (too_large) {
		ret |= DrawRT2ScreenLarge(dlist, tex_id, spr, rp, true);
	} else {
		ret |= DrawRT2ScreenSmall(dlist, tex_id, spr, rp, true);
	}
	return ret;
}

pt2::RenderReturn DrawPingPong::DrawRT2ScreenSmall(cooking::DisplayList* dlist, int tex_id, const Sprite* spr,
											  const RenderParams& rp, bool reset_color) const
{
	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();

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

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
#else
	cooking::change_shader(dlist, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED
	if (reset_color) 
	{
#ifdef S2_DISABLE_DEFERRED
		shader->SetColor(0xffffffff, 0);
		shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
		shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
#else
		cooking::set_color_sprite(dlist, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
		cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED
	} 
	else 
	{
		pt2::RenderColorCommon col_common;
		pt2::RenderColorMap    col_map;
		Utility::PrepareColor(rp.col_common, rp.col_map, spr, rp.actor, col_common, col_map);
#ifdef S2_DISABLE_DEFERRED
		shader->SetColor(col_common.mul.ToABGR(), col_common.add.ToABGR());
		shader->SetColorMap(col_map.rmap.ToABGR(), col_map.gmap.ToABGR(), col_map.bmap.ToABGR());
		shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
#else
		cooking::set_color_sprite(dlist, col.GetMulABGR(), col.GetAddABGR(),
			col.GetRMapABGR(), col.GetGMapABGR(), col.GetBMapABGR());
		cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED
	}

	return pt2::RENDER_OK;
}

pt2::RenderReturn DrawPingPong::DrawRT2ScreenLarge(cooking::DisplayList* dlist, int tex_id, const Sprite* spr,
											  const RenderParams& rp, bool reset_color) const
{
	pt2::RenderCtxStack::Instance()->Push(pt2::RenderContext(2, 2, 0, 0));

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

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
#else
	cooking::change_shader(dlist, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED
	if (reset_color) 
	{
#ifdef S2_DISABLE_DEFERRED
		shader->SetColor(0xffffffff, 0);
		shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
		shader->DrawQuad(vertices, texcoords, tex_id);
#else
		cooking::set_color_sprite(dlist, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
		cooking::draw_quad_sprite(dlist, vertices, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED
	} 
	else 
	{
		pt2::RenderColorCommon col_common;
		pt2::RenderColorMap    col_map;
		Utility::PrepareColor(rp.col_common, rp.col_map, spr, rp.actor, col_common, col_map);
#ifdef S2_DISABLE_DEFERRED
		shader->SetColor(col_common.mul.ToABGR(), col_common.add.ToABGR());
		shader->SetColorMap(col_map.rmap.ToABGR(), col_map.gmap.ToABGR(), col_map.bmap.ToABGR());
		shader->DrawQuad(vertices, texcoords, tex_id);
#else
		cooking::set_color_sprite(dlist, col.GetMulABGR(), col.GetAddABGR(),
			col.GetRMapABGR(), col.GetGMapABGR(), col.GetBMapABGR());
		cooking::draw_quad_sprite(dlist, vertices, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED
	}

	pt2::RenderCtxStack::Instance()->Pop();

	return pt2::RENDER_OK;
}

}