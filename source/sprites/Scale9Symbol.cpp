#include "sprite2/Scale9Symbol.h"
#include "sprite2/Scale9Sprite.h"
#include "sprite2/Scale9Actor.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#include "sprite2/SymbolVisitor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>

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
	CU_VEC<SprPtr> grids;
	m_s9.GetGrids(grids);
	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]) {
			visitor.Visit(grids[i]);
		}
	}
}

pt2::RenderReturn Scale9Symbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_SCALE9);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS

	pt2::RenderReturn ret = pt2::RENDER_OK;
	if (rp.actor) {
		RenderParamsProxy rp_proxy;
	    RenderParams* rp_child = rp_proxy.obj;
		memcpy(rp_child, &rp, sizeof(rp));

		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			ret = S2_VI_DOWN_CAST<const Scale9Actor*>(rp.actor)->GetScale9().Draw(dlist, *rp_child);
		} else {
			ret = pt2::RENDER_INVISIBLE;
		}
	} else if (spr) {
		RenderParamsProxy rp_proxy;
	    RenderParams* rp_child = rp_proxy.obj;
		memcpy(rp_child, &rp, sizeof(rp));

		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			ret = S2_VI_DOWN_CAST<const Scale9Sprite*>(spr)->GetScale9().Draw(dlist, *rp_child);
		} else {
			ret = pt2::RENDER_INVISIBLE;
		}
	} else {
		ret = m_s9.Draw(dlist, rp);
	}
	return ret;
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