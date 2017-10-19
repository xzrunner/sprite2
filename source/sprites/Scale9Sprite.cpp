#include "Scale9Sprite.h"
#include "Scale9Symbol.h"
#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>

namespace s2
{

Scale9Sprite::Scale9Sprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

Scale9Sprite::Scale9Sprite(const Scale9Sprite& spr)
	: Sprite(spr)
	, m_s9(spr.m_s9)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

Scale9Sprite& Scale9Sprite::operator = (const Scale9Sprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

Scale9Sprite::Scale9Sprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_s9(S2_VI_PTR_DOWN_CAST<Scale9Symbol>(sym)->GetScale9())
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

Scale9Sprite::~Scale9Sprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_SCALE9);
#endif // S2_DISABLE_STATISTICS
}

VisitResult Scale9Sprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	CU_VEC<SprPtr> grids;
	m_s9.GetGrids(grids);
	SprVisitorParams cp = params;
	if (visitor.GetOrder()) {
		for (int i = 0, n = grids.size(); i < n; ++i) 
		{
			auto& child = grids[i];
			cp.actor = child->QueryActorRef(params.actor.get());
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	} else {
		for (int i = grids.size() - 1; i >= 0; --i) 
		{
			auto& child = grids[i];
			cp.actor = child->QueryActorRef(params.actor.get());
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	}
	return ret;
}

void Scale9Sprite::Resize(float width, float height)
{
	m_s9.SetSize(width, height);
}

}