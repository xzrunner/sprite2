#include "PointQueryVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "S2_Actor.h"
#include "SprVisitorParams.h"
#include "ActorLUT.h"

#include "S2_RVG.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos)
	: SprVisitor(false)
	, m_pos(pos)
	, m_spr(NULL)
	, m_layer_find(false)
{
}

PointQueryVisitor::~PointQueryVisitor()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

VisitResult PointQueryVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
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
	if (sz.Width() == 0 || sz.Height() == 0) {
		return VISIT_CONTINUE;
	}
	std::vector<sm::vec2> vertices(4);
	vertices[0] = sm::vec2(sz.xmin, sz.ymin);
	vertices[1] = sm::vec2(sz.xmin, sz.ymax);
	vertices[2] = sm::vec2(sz.xmax, sz.ymax);
	vertices[3] = sm::vec2(sz.xmax, sz.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = params.mt * vertices[i];
	}
	RVG::Polyline(vertices, true);
	if (sm::is_point_in_convex(m_pos, vertices)) 
	{
		cu::RefCountObjAssign(m_spr, spr);
		m_mat = params.mt;
		m_layer_find = true;
		return VISIT_STOP;
	}
	else
	{
		return VISIT_CONTINUE;
	}
}

void PointQueryVisitor::VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params)
{
	m_layer_find = false;
}

void PointQueryVisitor::VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params)
{
	if (m_layer_find && !m_spr->IsEditable()) {		
		cu::RefCountObjAssign(m_spr, spr);
		m_mat = params.mt;
	}
}

}