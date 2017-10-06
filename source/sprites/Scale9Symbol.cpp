#include "Scale9Symbol.h"
#include "Scale9Sprite.h"
#include "Scale9Actor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "SymbolVisitor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <assert.h>

namespace s2
{

Scale9Symbol::Scale9Symbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

Scale9Symbol::Scale9Symbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

Scale9Symbol::~Scale9Symbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

int Scale9Symbol::Type() const 
{ 
	return SYM_SCALE9; 
}

void Scale9Symbol::Traverse(const SymbolVisitor& visitor)
{
	std::vector<SprPtr> grids;
	m_s9.GetGrids(grids);
	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]) {
			visitor.Visit(grids[i]);
		}
	}
}

RenderReturn Scale9Symbol::DrawTree(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_SCALE9);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS

	RenderReturn ret = RENDER_OK;
	if (rp.actor) {
		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			ret = std::static_pointer_cast<const Scale9Actor>(rp.actor)->GetScale9().Draw(*rp_child);
		} else {
			ret = RENDER_INVISIBLE;
		}
		RenderParamsPool::Instance()->Push(rp_child); 
	} else if (spr) {
		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			ret = S2_VI_DOWN_CAST<const Scale9Sprite*>(spr)->GetScale9().Draw(*rp_child);
		} else {
			ret = RENDER_INVISIBLE;
		}
		RenderParamsPool::Instance()->Push(rp_child); 
	} else {
		ret = m_s9.Draw(rp);
	}
	return ret;
}

RenderReturn Scale9Symbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	return RENDER_SKIP;
}

void Scale9Symbol::Resize(float width, float height)
{
	m_s9.SetSize(width, height);
}

sm::rect Scale9Symbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	sm::vec2 sz;
	if (actor) {
		sz = static_cast<const Scale9Actor*>(actor)->GetScale9().GetSize();
	} else if (spr) {
		sz = S2_VI_DOWN_CAST<const Scale9Sprite*>(spr)->GetScale9().GetSize();
	} else {
		sz = m_s9.GetSize();
	}
	return sm::rect(sz.x, sz.y);
}

}