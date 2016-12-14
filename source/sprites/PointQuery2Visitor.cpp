#include "PointQuery2Visitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "ActorFactory.h"

#include "S2_RVG.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

PointQuery2Visitor::PointQuery2Visitor(const SprTreePath& parent, const sm::vec2& pos)
	: SprVisitor(false)
	, m_parent(parent)
	, m_pos(pos)
	, m_spr(NULL)
	, m_layer_find(false)
{
}

PointQuery2Visitor::~PointQuery2Visitor()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

VisitResult PointQuery2Visitor::Visit(const Sprite* spr, const sm::mat4* mat)
{
	if (!spr->IsVisible()) {
		return VISIT_CONTINUE;
	}

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	assert(mat);
	sm::mat4 m = spr->GetLocalMat() * (*mat);
	sm::rect sz = spr->GetSymbol()->GetBounding(spr);
	std::vector<sm::vec2> vertices(4);
	vertices[0] = sm::vec2(sz.xmin, sz.ymin);
	vertices[1] = sm::vec2(sz.xmin, sz.ymax);
	vertices[2] = sm::vec2(sz.xmax, sz.ymax);
	vertices[3] = sm::vec2(sz.xmax, sz.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = m * vertices[i];
	}
	RVG::Polyline(vertices, true);
	if (sm::is_point_in_convex(m_pos, vertices)) 
	{
		cu::RefCountObjAssign(m_spr, spr);
		m_mat = m;
		m_layer_find = true;
		return VISIT_STOP;
	}
	else
	{
		return VISIT_CONTINUE;
	}
}

void PointQuery2Visitor::VisitChildrenBegin(const Sprite* spr)
{
	m_parent.Push(spr->GetID());
	m_layer_find = false;
}

void PointQuery2Visitor::VisitChildrenEnd(const Sprite* spr)
{
	assert(!m_parent.Empty() && m_parent.Top() == spr->GetID());

	if (m_layer_find && !m_spr->IsEditable()) {
		cu::RefCountObjAssign(m_spr, spr);
	}

	m_parent.Pop();
}

Actor* PointQuery2Visitor::GetSelectedActor() const
{
	if (!m_spr) {
		return NULL;
	}
	return ActorFactory::Instance()->Create(m_parent, m_spr);
}

}