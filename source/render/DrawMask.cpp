#include "sprite2/DrawMask.h"
#include "sprite2/Actor.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#ifndef S2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#include "sprite2/StatOverdraw.h"
#include "sprite2/Blackboard.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <SM_Rect.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shader.h>
#include <shaderlab/MaskShader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/RenderTargetMgr.h>
#include <painting2/RenderTarget.h>
#include <painting2/RenderCtxStack.h>
#include <painting2/RenderScissor.h>

namespace s2
{

RenderReturn DrawMask::Draw(cooking::DisplayList* dlist, const Sprite* base, const Sprite* mask, const RenderParams& rp)
{
	auto base_actor = base->QueryActor(rp.actor);
	bool visible = base_actor ? base_actor->IsVisible() : base->IsVisible();
	if (!visible) {
		return RENDER_INVISIBLE;
	}
	auto mask_actor = mask->QueryActor(rp.actor);
	visible = mask_actor ? mask_actor->IsVisible() : mask->IsVisible();
	if (!visible) {
		return RENDER_INVISIBLE;
	}

	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::MASK);
#endif // S2_DISABLE_STATISTICS

	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	pt2::RenderScissor::Instance()->Disable();
	pt2::RenderCtxStack::Instance()->Push(pt2::RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	pt2::RenderTarget* rt_base = RT->Fetch();
	if (!rt_base) {
		pt2::RenderCtxStack::Instance()->Pop();
		pt2::RenderScissor::Instance()->Enable();
		return RENDER_NO_RT;
	}
	ret |= DrawBaseToRT(dlist, rt_base, base, base_actor, rp);

	pt2::RenderTarget* rt_mask = RT->Fetch();
	if (!rt_mask) {
		RT->Return(rt_base);
		pt2::RenderCtxStack::Instance()->Pop();
		pt2::RenderScissor::Instance()->Enable();
		return RENDER_NO_RT;
	}
	ret |= DrawMaskToRT(dlist, rt_mask, mask, mask_actor, rp);

	pt2::RenderCtxStack::Instance()->Pop();
	pt2::RenderScissor::Instance()->Enable();

	ret |= DrawMaskFromRT(dlist, rt_base, rt_mask, mask, rp.mt);

	RT->Return(rt_base);
	RT->Return(rt_mask);

	return ret;
}

//RenderReturn DrawMask::DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp)
//{
//	const Actor* base_actor = base->QueryActor(rp.actor);
//	bool visible = base_actor ? base_actor->IsVisible() : base->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//	const Actor* mask_actor = mask->QueryActor(rp.actor);
//	visible = mask_actor ? mask_actor->IsVisible() : mask->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//
//	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
//
//	rc->ClearAll();
//
//	rc->ColorMask(false, false, false, true);
//
//	rc->EnableStencil(true);
//	rc->EnableAlpha(true);
//
//	rc->StencilFunc(ur::RenderContext::ST_ALWAYS, 0, 0);
//	rc->StencilOp(ur::RenderContext::ST_KEEP, ur::RenderContext::ST_KEEP, ur::RenderContext::ST_INCR);	
//	rc->AlphaFunc();
//
//	RenderParams rp_child = rp;
//	rp_child.actor = mask_actor;
//	DrawNode::Draw(mask, rp_child);
//	sl::ShaderMgr::Instance()->FlushShader();
//
//	rc->EnableAlpha(false);
//	
//	rc->ColorMask(true, true, true, true);
//	
//// 	rc->StencilFunc(ur::RenderContext::ST_LESS, 1, -1);
//	rc->StencilFunc(ur::RenderContext::ST_NEVER, 0, 0);
//
// 	rp_child.actor = base_actor;
// 	DrawNode::Draw(base, rp_child);
// 	sl::ShaderMgr::Instance()->FlushShader();
//
//	rc->EnableStencil(false);
//
//	return RENDER_OK;
//}

//RenderReturn DrawMask::DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp)
//{
//	const Actor* base_actor = base->QueryActor(rp.actor);
//	bool visible = base_actor ? base_actor->IsVisible() : base->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//	const Actor* mask_actor = mask->QueryActor(rp.actor);
//	visible = mask_actor ? mask_actor->IsVisible() : mask->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//
//	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
//
//	rc->EnableStencil(true);
//	rc->ClearStencil(0);
//
//	rc->ClearAll();
//	{
//		rc->ColorMask(false, false, false, true);
//
//		rc->StencilFunc(ur::RenderContext::ST_NEVER, 0, 0);
//		rc->StencilOp(ur::RenderContext::ST_INCR, ur::RenderContext::ST_INCR, ur::RenderContext::ST_INCR);
//
//		rc->EnableAlpha(true);
//		rc->AlphaFunc();
//
//		RenderParams rp_child = rp;
//		rp_child.actor = mask_actor;
//		DrawNode::Draw(mask, rp_child);
//		sl::ShaderMgr::Instance()->FlushShader();
//
//		sl::ShaderMgr::Instance()->FlushShader();
//
//		rc->EnableAlpha(false);
//		rc->ColorMask(true, true, true, true);
//	}
//
//	{
//		rc->StencilFunc(ur::RenderContext::ST_NOTEQUAL, 0, 1);
//		rc->StencilOp(ur::RenderContext::ST_KEEP, ur::RenderContext::ST_KEEP, ur::RenderContext::ST_KEEP);
//
//		RenderParams rp_child = rp;
//		rp_child.actor = base_actor;
//		DrawNode::Draw(base, rp_child);
//		sl::ShaderMgr::Instance()->FlushShader();
//
//		sl::ShaderMgr::Instance()->FlushShader();
//	}
//
//	rc->EnableStencil(false);
//
//	return RENDER_OK;
//}

RenderReturn DrawMask::DrawBaseToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, const Sprite* base,
									const Actor* actor, const RenderParams& rp)
{
	rt->Bind();

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();
#else
	cooking::render_clear(dlist, 0);
	cooking::change_shader(dlist, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	rp_child->Reset();

	rp_child->col_common = rp.col_common;
	rp_child->col_map    = rp.col_map;
	rp_child->actor = actor;
	rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
//	rp_child->mt = rp.mt;

	RenderReturn ret = DrawNode::Draw(nullptr, base, *rp_child);

#ifdef S2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	rt->Unbind();

	return ret;
}

RenderReturn DrawMask::DrawMaskToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, const Sprite* mask,
									const Actor* actor, const RenderParams& rp)
{
	rt->Bind();

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();
#else
	cooking::render_clear(dlist, 0);
	cooking::change_shader(dlist, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	rp_child->Reset();

	rp_child->SetChangeShader(false);
	rp_child->actor = actor;
	rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
//	rp_child->mt = rp.mt;

	RenderReturn ret = DrawNode::Draw(nullptr, mask, *rp_child);

#ifdef S2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	rt->Unbind();

	return ret;
}

RenderReturn DrawMask::DrawMaskFromRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt_base, pt2::RenderTarget* rt_mask,
									  const Sprite* mask, const S2_MAT& mt)
{
	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();

	sm::vec2 vertices[4];
	sm::rect r = mask->GetSymbol()->GetBounding();
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);

	sm::vec2 texcoords[4];
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i];
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

	sm::vec2 texcoords_mask[4];
	for (int i = 0; i < 4; ++i) {
		texcoords_mask[i] = vertices[i];
		texcoords_mask[i].x = texcoords_mask[i].x / RT->WIDTH  + 0.5f;
		texcoords_mask[i].y = texcoords_mask[i].y / RT->HEIGHT + 0.5f;
	}

	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

#ifndef S2_DISABLE_STATISTICS
	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax =-FLT_MAX, ymax =-FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		const sm::vec2& pos = vertices[i];
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	const sm::ivec2& sz = Blackboard::Instance()->GetScreenSize();	
	float area = (xmax - xmin) * (ymax - ymin) / sz.x / sz.y;
	StatOverdraw::Instance()->AddArea(area);
#endif // S2_DISABLE_STATISTICS

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
#else
	cooking::change_shader(dlist, sl::MASK);
	cooking::draw_quad_mask(dlist, &vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
#endif // S2_DISABLE_DEFERRED

	return RENDER_OK;
}

}