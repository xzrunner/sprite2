#include "BoundingDirtyVisitor.h"
#include "S2_Symbol.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "SprVisitorParams.h"
#include "SymType.h"

namespace s2
{

BoundingDirtyVisitor::BoundingDirtyVisitor()
	: m_bounding_dirty(false)
{
}

VisitResult BoundingDirtyVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	if (m_bounding_dirty) {
		return VISIT_STOP;
	}

	bool visible = spr->IsVisible();
	if (spr->HaveActor()) {
		const Actor* actor = params.actor;
		if (actor) {
			visible = actor->IsVisible();
		}
	}
	if (!visible) {
		return VISIT_OVER;
	}

	VisitResult ret = VISIT_OVER;

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	switch (type)
	{
	case SYM_INVALID: case SYM_UNKNOWN:
		ret = VISIT_OVER;
		break;
	case SYM_COMPLEX: case SYM_ANIMATION:
		ret = VISIT_INTO;
		break;
	default:
		if (spr->IsBoundingDirty()) {
			m_bounding_dirty = true;
			ret = VISIT_STOP;
		}
	}

	return ret;
}

}