#include "Scale9Symbol.h"
#include "Scale9Sprite.h"
#include "Scale9Actor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "SymbolVisitor.h"
#include "FlattenParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymbol.h"
#endif // S2_DISABLE_STATISTICS

#include <assert.h>

namespace s2
{

Scale9Symbol::Scale9Symbol()
{
}

Scale9Symbol::Scale9Symbol(uint32_t id)
	: Symbol(id)
{
}

int Scale9Symbol::Type() const 
{ 
	return SYM_SCALE9; 
}

void Scale9Symbol::Traverse(const SymbolVisitor& visitor)
{
	std::vector<Sprite*> grids;
	m_s9.GetGrids(grids);
	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]) {
			visitor.Visit(grids[i]);
		}
	}
}

RenderReturn Scale9Symbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymbol::Instance()->AddDrawCount(StatSymbol::SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS

	RenderReturn ret = RENDER_OK;
	if (rp.actor) {
		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			ret = VI_DOWNCASTING<const Scale9Actor*>(rp.actor)->GetScale9().Draw(*rp_child);
		} else {
			ret = RENDER_INVISIBLE;
		}
		RenderParamsPool::Instance()->Push(rp_child); 
	} else if (spr) {
		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			ret = VI_DOWNCASTING<const Scale9Sprite*>(spr)->GetScale9().Draw(*rp_child);
		} else {
			ret = RENDER_INVISIBLE;
		}
		RenderParamsPool::Instance()->Push(rp_child); 
	} else {
		ret = m_s9.Draw(rp);
	}
	return ret;
}

void Scale9Symbol::Flattening(const FlattenParams& fp, Flatten& ft) const
{
	if (fp.GetActor()) {
		VI_DOWNCASTING<const Scale9Actor*>(fp.GetActor())->GetScale9().Flattening(fp, ft);
	} else if (fp.GetSpr()) {
		VI_DOWNCASTING<const Scale9Sprite*>(fp.GetSpr())->GetScale9().Flattening(fp, ft);
	} else {
		m_s9.Flattening(fp, ft);
	}
}

void Scale9Symbol::Resize(float width, float height)
{
	m_s9.SetSize(width, height);
}

sm::rect Scale9Symbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	sm::vec2 sz;
	if (actor) {
		sz = VI_DOWNCASTING<const Scale9Actor*>(actor)->GetScale9().GetSize();
	} else if (spr) {
		sz = VI_DOWNCASTING<const Scale9Sprite*>(spr)->GetScale9().GetSize();
	} else {
		sz = m_s9.GetSize();
	}
	return sm::rect(sz.x, sz.y);
}

}