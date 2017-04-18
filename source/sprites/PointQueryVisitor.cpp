#include "PointQueryVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "SprVisitorParams.h"
#include "AnchorSprite.h"
#include "ComplexSymbol.h"

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
	} else if (type == SYM_ANCHOR) {
		const AnchorSprite* anchor_spr = VI_DOWNCASTING<const AnchorSprite*>(spr);
		const Actor* real = anchor_spr->QueryAnchor(actor);
		if (real) {
			SprVisitorParams cp = params;
			cp.actor = real->GetSpr()->QueryActor(actor);
			return Visit(real->GetSpr(), cp);
		} else {
			return VISIT_OVER;
		}
	}

	if (!QuerySprite(spr, params)) {
		return VISIT_OVER;
	}
	
	bool should_into = false;
	if (type == SYM_COMPLEX || type == SYM_ANIMATION || type == SYM_SCALE9) {
		return VISIT_INTO;
	}

	bool editable = actor ? actor->IsEditable() : spr->IsEditable();
	if (editable) 
	{
		cu::RefCountObjAssign(m_selected_spr, spr);
		m_selected_params = params;
		m_selected_path = m_curr_path;
		return VISIT_STOP;
	} 
	else 
	{
		bool use_new = false;
		if (!m_selected_spr || m_selected_path.IsPartOf(m_curr_path)) {
			use_new = true;
		}
		if (m_selected_spr) {
			if (!m_selected_path.IsEditable() && m_curr_path.IsEditable()) {
				use_new = true;
			}
		}

		if (use_new) {
			cu::RefCountObjAssign(m_selected_spr, spr);
			m_selected_params = params;
			m_selected_path = m_curr_path;
			return VISIT_OVER;
		} else {
			return VISIT_OVER;
		}
	}
}

VisitResult PointQueryVisitor::VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params)
{
	bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
	m_curr_path.Push(spr->GetID(), editable);
	return VISIT_OVER;
}

VisitResult PointQueryVisitor::VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params)
{
	VisitResult ret = VISIT_OVER;

	if (m_selected_path.Empty())
	{
		SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
		if (type == SYM_COMPLEX) 
		{
			const ComplexSymbol* comp_sym = VI_DOWNCASTING<const ComplexSymbol*>(spr->GetSymbol());
			sm::vec2 scissor_sz = comp_sym->GetScissor().Size();
			if (scissor_sz.x > 0 && scissor_sz.y > 0) 
			{
				bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
				cu::RefCountObjAssign(m_selected_spr, spr);
				m_selected_params = params;
				m_selected_path = m_curr_path;
				ret = editable ? VISIT_STOP : VISIT_OVER;
			}
		}
	}
	else if (m_curr_path.IsPartOf(m_selected_path))
	{
		bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
		if (editable) 
		{
			cu::RefCountObjAssign(m_selected_spr, spr);
			m_selected_params = params;
			m_selected_path = m_curr_path;
			ret = VISIT_STOP;
		}
	} 

 	assert(!m_curr_path.Empty());
 	m_curr_path.Pop();

	return ret;
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
	sm::rect sz = spr->GetSymbol()->GetBounding(spr, params.actor);
	if (sz.Width() == 0 || sz.Height() == 0 || !sz.IsValid()) {
		return false;
	}

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(sz.xmin, sz.ymin);
	vertices[1] = sm::vec2(sz.xmin, sz.ymax);
	vertices[2] = sm::vec2(sz.xmax, sz.ymax);
	vertices[3] = sm::vec2(sz.xmax, sz.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = params.mt * vertices[i];
	}

	return sm::is_point_in_convex(m_pos, vertices, 4);
}

/************************************************************************/
/* class PointQueryVisitor::SprPath                                     */
/************************************************************************/

bool PointQueryVisitor::SprPath::
IsPartOf(const SprPath& long_path) const
{
	return m_impl.IsPartOf(long_path.m_impl);
}

bool PointQueryVisitor::SprPath::
Empty() const
{
	return m_impl.Empty();
}

void PointQueryVisitor::SprPath::
Push(int spr_id, bool editable)
{
	m_impl.Push(spr_id);
	m_editable.push_back(editable);
}

void PointQueryVisitor::SprPath::
Pop()
{
	m_impl.Pop();
	if (!m_editable.empty()) {
		m_editable.pop_back();
	}
}

bool PointQueryVisitor::SprPath::
IsEditable() const
{
	for (int i = 0, n = m_editable.size(); i < n; ++i) {
		if (m_editable[i]) {
			return true;
		}
	}
	return false;
}

}