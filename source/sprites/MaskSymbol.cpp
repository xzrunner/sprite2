#include "sprite2/MaskSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawMaskFT.h"
#include "sprite2/DrawNode.h"
#include "sprite2/SymbolVisitor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#ifndef S2_DISABLE_FLATTEN
#include <flatten/FTNode.h>
#endif // S2_DISABLE_FLATTEN
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/DrawMask.h>

namespace
{

class DrawMask : public pt2::DrawMask<s2::Sprite, s2::RenderParams>
{
public:
	DrawMask(const s2::Sprite& base, const s2::Sprite& mask, const s2::RenderParams& rp)
		: pt2::DrawMask<s2::Sprite, s2::RenderParams>(base, mask, rp) 
	{
		m_base_actor = base.QueryActor(rp.actor);
		m_mask_actor = mask.QueryActor(rp.actor);
	}

	virtual pt2::RenderReturn Draw(cooking::DisplayList* dlist) const override
	{
		bool visible = m_base_actor ? m_base_actor->IsVisible() : m_base.IsVisible();
		if (!visible) {
			return pt2::RENDER_INVISIBLE;
		}
		visible = m_mask_actor ? m_mask_actor->IsVisible() : m_mask.IsVisible();
		if (!visible) {
			return pt2::RENDER_INVISIBLE;
		}

		return this->DrawImpl(dlist);
	}

protected:
	virtual pt2::RenderReturn DrawBaseNode(const s2::Sprite& node, const s2::RenderParams& rp) const override
	{
		s2::RenderParamsProxy rp_proxy;
		s2::RenderParams* rp_child = rp_proxy.obj;
		rp_child->Reset();

		rp_child->col_common = rp.col_common;
		rp_child->col_map    = rp.col_map;
		rp_child->actor      = m_base_actor;
		rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
	//	rp_child->mt = rp.mt;

		return s2::DrawNode::Draw(nullptr, &node, *rp_child);
	}
	virtual pt2::RenderReturn DrawMaskNode(const s2::Sprite& node, const s2::RenderParams& rp) const override
	{
		s2::RenderParamsProxy rp_proxy;
		s2::RenderParams* rp_child = rp_proxy.obj;
		rp_child->Reset();

		rp_child->SetChangeShader(false);
		rp_child->actor = m_mask_actor;
		rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
	//	rp_child->mt = rp.mt;

		return s2::DrawNode::Draw(nullptr, &node, *rp_child);
	}

	virtual sm::rect GetBounding(const s2::Sprite& spr) const override
	{
		return spr.GetSymbol()->GetBounding();
	}

	const sm::Matrix2D& GetMat(const s2::RenderParams& rp) const override
	{
		return rp.mt;
	}

private:
	s2::Actor *m_base_actor, *m_mask_actor;

}; // DrawMask

}

namespace s2
{

MaskSymbol::MaskSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

MaskSymbol::MaskSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

MaskSymbol::~MaskSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

int MaskSymbol::Type() const 
{ 
	return SYM_MASK; 
}

void MaskSymbol::Traverse(const SymbolVisitor& visitor)
{
	if (m_base) {
		visitor.Visit(m_base);
	}
	if (m_mask) {
		visitor.Visit(m_mask);
	}
}

pt2::RenderReturn MaskSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_MASK);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}
	pt2::RenderReturn ret = pt2::RENDER_OK;
	if (m_base && m_mask) {
//		ret = DrawMask::Draw(dlist, m_base.get(), m_mask.get(), *rp_child);

		DrawMask draw(*m_base, *m_mask, *rp_child);
		ret = draw.Draw(dlist);

//		ret = DrawMask::DrawByStencil(m_base, m_mask, *rp_child);
	} else {
		if (m_base) {
			rp_child->actor = m_base->QueryActor(rp.actor);
			ret = DrawNode::Draw(dlist, m_base.get(), *rp_child);
		} else if (m_mask) {
			rp_child->actor = m_mask->QueryActor(rp.actor);
			ret = DrawNode::Draw(dlist, m_mask.get(), *rp_child);
		}
	}

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn MaskSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, 
	                              ft::FTList& ft, int pos) const
{
	if (m_base && m_mask)
	{
		int n_tot = ft.GetNode(pos)->GetCount();
		int n_base = ft.GetNode(pos + 1)->GetCount();
		int n_mask = ft.GetNode(pos + 1 + n_base)->GetCount();
		assert(n_tot - 1 == n_base + n_mask);
		DrawMaskFT::Draw(ft, pos + 1, pos + 1 + n_base, rp);
	}
	else if (m_base || m_mask)
	{
		ft.DrawForward(pos + 1, rp);
	}
	return pt2::RENDER_OK;
}
#endif // S2_DISABLE_FLATTEN

sm::rect MaskSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (m_mask) {
		sm::rect b;
		m_mask->GetBounding().CombineTo(b);
		return b;
	} else {
		return sm::rect(sm::vec2(0, 0), 100, 100);
	}
}

}