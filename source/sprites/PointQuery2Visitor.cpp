#include "PointQuery2Visitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "SprVisitorParams.h"
#include "ActorLUT.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

PointQuery2Visitor::PointQuery2Visitor(const sm::vec2& pos)
	: SprVisitor(false)
	, m_pos(pos)
	, m_selected_spr(NULL)
	, m_layer_find(false)
{
}

PointQuery2Visitor::~PointQuery2Visitor()
{
	if (m_selected_spr) {
		m_selected_spr->RemoveReference();
	}
}

VisitResult PointQuery2Visitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	if (!spr->IsVisible()) {
		return VISIT_CONTINUE;
	}
	
	Actor* actor = ActorLUT::Instance()->Query(params.path);
	if (actor && !actor->IsVisible()) {
		return VISIT_CONTINUE;
	}

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	sm::rect sz = spr->GetSymbol()->GetBounding(spr);
	std::vector<sm::vec2> vertices(4);
	vertices[0] = sm::vec2(sz.xmin, sz.ymin);
	vertices[1] = sm::vec2(sz.xmin, sz.ymax);
	vertices[2] = sm::vec2(sz.xmax, sz.ymax);
	vertices[3] = sm::vec2(sz.xmax, sz.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = params.mt * vertices[i];
	}

	if (sm::is_point_in_convex(m_pos, vertices)) 
	{
		cu::RefCountObjAssign(m_selected_spr, spr);
		m_selected_params = params;
		m_layer_find = true;
		return VISIT_STOP;
	}
	else
	{
		return VISIT_CONTINUE;
	}
}

void PointQuery2Visitor::VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params)
{
	m_layer_find = false;
}

void PointQuery2Visitor::VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params)
{
	if (m_layer_find && !m_selected_spr->IsEditable()) {
		cu::RefCountObjAssign(m_selected_spr, spr);
		m_selected_params = params;
	}
}

Actor* PointQuery2Visitor::GetSelectedActor() const
{
	if (!m_selected_spr) {
		return NULL;
	}
	return ActorLUT::Instance()->Query(m_selected_params.path);
}

}