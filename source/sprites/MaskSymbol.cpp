#include "sprite2/MaskSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawMask.h"
#include "sprite2/DrawMaskFT.h"
#include "sprite2/DrawNode.h"
#include "sprite2/BoundingBox.h"
#include "sprite2/SymbolVisitor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <flatten/FTNode.h>
#include <cooking/DisplayList.h>

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

RenderReturn MaskSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_MASK);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}
	RenderReturn ret = RENDER_OK;
	if (m_base && m_mask) {
		ret = DrawMask::Draw(m_base.get(), m_mask.get(), *rp_child);
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
RenderReturn MaskSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, 
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
	return RENDER_OK;
}
#endif // S2_DISABLE_FLATTEN

sm::rect MaskSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (m_mask) {
		sm::rect b;
		m_mask->GetBounding()->CombineTo(b);
		return b;
	} else {
		return sm::rect(sm::vec2(0, 0), 100, 100);
	}
}

}