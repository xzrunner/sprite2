#include "DrawMaskFT.h"
#include "S2_Actor.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderScissor.h"
#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#include "sprite2/StatOverdraw.h"
#include "sprite2/Blackboard.h"
#endif // S2_DISABLE_STATISTICS

#include <flatten/FTNode.h>

#include <SM_Rect.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shader.h>
#include <shaderlab/MaskShader.h>

namespace s2
{

RenderReturn DrawMaskFT::Draw(ft::FTList& ft, int base, int mask, const RenderParams& rp)
{
	const ft::FTNode* ft_base = ft.GetNode(base);
	const ft::FTNode* ft_mask = ft.GetNode(mask);
	if (!ft_base || !ft_mask) {
		return RENDER_NO_DATA;
	}

	ActorConstPtr base_actor = nullptr;
	if (ft_base->IsDataSpr()) {
		auto spr(ft_base->GetSpr());
		if (!spr->IsVisible()) {
			return RENDER_INVISIBLE;
		}
	} else {
		auto actor(ft_base->GetActor());
		if (!actor->IsVisible()) {
			return RENDER_INVISIBLE;
		} else {
			base_actor = actor;
		}
	}

	ActorConstPtr mask_actor = nullptr;
	if (ft_mask->IsDataSpr()) {
		auto spr(ft_mask->GetSpr());
		if (!spr->IsVisible()) {
			return RENDER_INVISIBLE;
		}
	} else {
		auto actor(ft_mask->GetActor());
		if (!actor->IsVisible()) {
			return RENDER_INVISIBLE;
		} else {
			mask_actor = actor;
		}
	}

	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::MASK);
#endif // S2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	RenderTarget* rt_base = RT->Fetch();
	if (!rt_base) {
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Enable();
		return RENDER_NO_RT;
	}
	ret |= DrawBaseToRT(rt_base, ft, base, base_actor, rp);

	RenderTarget* rt_mask = RT->Fetch();
	if (!rt_mask) {
		RT->Return(rt_base);
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Enable();
		return RENDER_NO_RT;
	}
	ret |= DrawMaskToRT(rt_mask, ft, mask, mask_actor, rp);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	ret |= DrawMaskFromRT(rt_base, rt_mask, ft, mask, rp.mt);

	RT->Return(rt_base);
	RT->Return(rt_mask);

	return ret;
}

RenderReturn DrawMaskFT::DrawBaseToRT(RenderTarget* rt, ft::FTList& ft, int base, 
	                                  const ActorConstPtr& actor, 
	                                  const RenderParams& rp)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	rp_child->color = rp.color;
	rp_child->actor = actor;
	rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
//	rp_child->mt = rp.mt;

	ft.DrawForward(base, *rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	shader->Commit();

	rt->Unbind();

	return RENDER_OK;
}

RenderReturn DrawMaskFT::DrawMaskToRT(RenderTarget* rt, ft::FTList& ft, int mask,
	                                  const ActorConstPtr& actor, 
	                                  const RenderParams& rp)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	rp_child->SetChangeShader(false);
	rp_child->actor = actor;
	rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
//	rp_child->mt = rp.mt;

	ft.DrawForward(mask, *rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	shader->Commit();

	rt->Unbind();

	return RENDER_OK;
}

RenderReturn DrawMaskFT::DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, 
	                                    ft::FTList& ft, int mask, const S2_MAT& mt)
{
	const ft::FTNode* ft_n = ft.GetNode(mask);
	if (!ft_n) {
		return RENDER_NO_DATA;
	}

	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	const Symbol* sym = nullptr;
	if (ft_n->IsDataSpr()) {
		sym = ft_n->GetSpr()->GetSymbol().get();
	} else {
		sym = ft_n->GetActor()->GetSpr()->GetSymbol().get();
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
	StatOverdraw::Instance()->AddArea(area);
#endif // S2_DISABLE_STATISTICS

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());

	return RENDER_OK;
}

}