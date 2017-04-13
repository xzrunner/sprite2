#include "Scale9Symbol.h"
#include "Scale9Sprite.h"
#include "Scale9Actor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "SymbolVisitor.h"

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

void Scale9Symbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (rp.actor) {
		RenderParams rp_child(rp);
		if (DrawNode::Prepare(rp, spr, rp_child)) {
			VI_DOWNCASTING<const Scale9Actor*>(rp.actor)->GetScale9().Draw(rp_child);
		}
	} else if (spr) {
		RenderParams rp_child(rp);
		if (DrawNode::Prepare(rp, spr, rp_child)) {
			VI_DOWNCASTING<const Scale9Sprite*>(spr)->GetScale9().Draw(rp_child);
		}
	} else {
		m_s9.Draw(rp);
	}
}

sm::rect Scale9Symbol::GetBounding(const Sprite* spr, const Actor* actor, bool cache) const
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

void Scale9Symbol::Resize(float width, float height)
{
	m_s9.SetSize(width, height);
}

}