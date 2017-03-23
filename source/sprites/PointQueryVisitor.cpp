#include "PointQueryVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "SprVisitorParams.h"
#include "ActorLUT.h"
#include "QueryAABBVisitor.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos)
	: SpriteVisitor(false)
	, m_pos(pos)
	, m_selected_spr(NULL)
{
}

PointQueryVisitor::~PointQueryVisitor()
{
	if (m_selected_spr) {
		m_selected_spr->RemoveReference();
	}
}

VisitResult PointQueryVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	if (!spr->IsVisible()) {
		return VISIT_OVER;
	}
	
	const Actor* actor = params.actor;
	if (actor && !actor->IsVisible()) {
		return VISIT_OVER;
	}

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_OVER;
	}
	if (!QuerySprite(spr, params)) {
		return VISIT_OVER;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	bool editable = spr->IsEditable();
	if (actor) {
		editable = actor->IsEditable();
	}
	if (editable) {
		cu::RefCountObjAssign(m_selected_spr, spr);
		m_selected_params = params;
		return VISIT_STOP;
	} else {
		if (!m_selected_spr) {
			cu::RefCountObjAssign(m_selected_spr, spr);
			m_selected_params = params;
			return VISIT_OVER;
		} else {
			return VISIT_OVER;
		}
	}
}

VisitResult PointQueryVisitor::VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params)
{
	return VISIT_OVER;
}

VisitResult PointQueryVisitor::VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params)
{
	bool editable = spr->IsEditable();
	const Actor* actor = params.actor;
	if (actor) {
		editable = actor->IsEditable();
	}
	if (editable) {
		cu::RefCountObjAssign(m_selected_spr, spr);
		m_selected_params = params;
		return VISIT_STOP;
	} else {
		return VISIT_OVER;
	}
}

const Actor* PointQueryVisitor::GetSelectedActor() const
{
	if (!m_selected_spr) {
		return NULL;
	}
	return m_selected_params.actor;
}

bool PointQueryVisitor::QuerySprite(const Sprite* spr, const SprVisitorParams& params) const
{
	sm::rect sz = spr->GetSymbol()->GetBounding(spr);
	if (sz.Width() == 0 || sz.Height() == 0) {
		return false;
	}

	SprVisitorParams p;
	p.actor = params.actor;
	QueryAABBVisitor visitor(true, false, true);
	spr->Traverse(visitor, p);

	return sm::is_point_in_rect(m_pos, visitor.GetAABB());
}

}