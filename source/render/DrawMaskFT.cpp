#include "sprite2/DrawMaskFT.h"

#ifndef S2_DISABLE_FLATTEN

#include "sprite2/Actor.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#ifndef S2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#include <stat/StatOverdraw.h>
#include "sprite2/Blackboard.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#ifndef S2_DISABLE_FLATTEN
#include <flatten/FTList.h>
#include <flatten/FTNode.h>
#endif // S2_DISABLE_FLATTEN
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

pt2::RenderReturn DrawMaskFT::Draw(cooking::DisplayList* dlist, ft::FTList& ft, 
	                          int base, int mask, const RenderParams& rp)
{
	const ft::FTNode* ft_base = ft.GetNode(base);
	const ft::FTNode* ft_mask = ft.GetNode(mask);
	if (!ft_base || !ft_mask) {
		return pt2::RENDER_NO_DATA;
	}

	const Actor* base_actor = nullptr;
	if (ft_base->IsDataSpr()) {
		auto spr(static_cast<const s2::Sprite*>(ft_base->GetData()));
		if (!spr->IsVisible()) {
			return pt2::RENDER_INVISIBLE;
		}
	} else {
		auto actor(static_cast<const s2::Actor*>(ft_base->GetData()));
		if (!actor->IsVisible()) {
			return pt2::RENDER_INVISIBLE;
		} else {
			base_actor = actor;
		}
	}

	const Actor* mask_actor = nullptr;
	if (ft_mask->IsDataSpr()) {
		auto spr(static_cast<const s2::Sprite*>(ft_mask->GetData()));
		if (!spr->IsVisible()) {
			return pt2::RENDER_INVISIBLE;
		}
	} else {
		auto actor(static_cast<const s2::Actor*>(ft_mask->GetData()));
		if (!actor->IsVisible()) {
			return pt2::RENDER_INVISIBLE;
		} else {
			mask_actor = actor;
		}
	}

	pt2::RenderReturn ret = pt2::RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::MASK);
#endif // S2_DISABLE_STATISTICS

	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.FlushShader();
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
		return pt2::RENDER_NO_RT;
	}
	ret |= DrawBaseToRT(dlist, rt_base, ft, base, base_actor, rp);

	pt2::RenderTarget* rt_mask = RT->Fetch();
	if (!rt_mask) {
		RT->Return(rt_base);
		pt2::RenderCtxStack::Instance()->Pop();
		pt2::RenderScissor::Instance()->Enable();
		return pt2::RENDER_NO_RT;
	}
	ret |= DrawMaskToRT(dlist, rt_mask, ft, mask, mask_actor, rp);

	pt2::RenderCtxStack::Instance()->Pop();
	pt2::RenderScissor::Instance()->Enable();

	ret |= DrawMaskFromRT(dlist, rt_base, rt_mask, ft, mask, rp.mt);

	RT->Return(rt_base);
	RT->Return(rt_mask);

	return ret;
}

pt2::RenderReturn DrawMaskFT::DrawBaseToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, ft::FTList& ft, 
	                                  int base, const Actor* actor, const RenderParams& rp)
{
	rt->Bind();

#ifdef S2_DISABLE_DEFERRED
	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	rc.GetContext().Clear(0);

	shader_mgr.SetShader(sl::SPRITE2);
	sl::Shader* shader = shader_mgr.GetShader();
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

	ft.DrawForward(base, *rp_child);

#ifdef S2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	rt->Unbind();

	return pt2::RENDER_OK;
}

pt2::RenderReturn DrawMaskFT::DrawMaskToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, ft::FTList& ft, 
	                                  int mask, const Actor* actor, const RenderParams& rp)
{
	rt->Bind();

#ifdef S2_DISABLE_DEFERRED
	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	rc.GetContext().Clear(0);

	rc.GetShaderMgr().SetShader(sl::SPRITE2);
	sl::Shader* shader = shader_mgr.GetShader();
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

	ft.DrawForward(mask, *rp_child);

#ifdef S2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	rt->Unbind();

	return pt2::RENDER_OK;
}

pt2::RenderReturn DrawMaskFT::DrawMaskFromRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt_base, 
	                                    pt2::RenderTarget* rt_mask, ft::FTList& ft, int mask, const S2_MAT& mt)
{
	const ft::FTNode* ft_n = ft.GetNode(mask);
	if (!ft_n) {
		return pt2::RENDER_NO_DATA;
	}

	pt2::RenderTargetMgr* RT = pt2::RenderTargetMgr::Instance();

	const Symbol* sym = nullptr;
	if (ft_n->IsDataSpr()) {
		sym = static_cast<const s2::Sprite*>(ft_n->GetData())->GetSymbol().get();
	} else {
		sym = static_cast<const s2::Actor*>(ft_n->GetData())->GetSprRaw()->GetSymbol().get();
	}

	sm::vec2 vertices[4];
	sm::rect r = sym->GetBounding();
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
	st::StatOverdraw::Instance()->AddArea(area);
#endif // S2_DISABLE_STATISTICS

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();

	shader_mgr.SetShader(sl::MASK);

	sl::MaskShader* shader = static_cast<sl::MaskShader*>(shader_mgr.GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
#else
	cooking::change_shader(dlist, sl::MASK);
	cooking::draw_quad_mask(dlist, &vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
#endif // S2_DISABLE_DEFERRED

	return pt2::RENDER_OK;
}

}

#endif // S2_DISABLE_FLATTEN