#include "PointQueryVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "SprVisitorParams.h"
#include "AnchorSprite.h"
#include "ComplexSymbol.h"
#include "ShapeSymbol.h"
#include "Shape.h"
#include "ActorProxy.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos)
	: SpriteVisitor2(false)
	, m_pos(pos)
	, m_selected_spr(nullptr)
	, m_finded(false)
{
}

VisitResult PointQueryVisitor::Visit(const SprConstPtr& spr, const SprVisitorParams2& params)
{
	bool visible = params.actor ? params.actor->IsVisible() : spr->IsVisible();
	if (!visible) {
		return VISIT_OVER;
	}

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_OVER;
	} else if (type == SYM_ANCHOR) {
		auto& anchor_spr = S2_VI_PTR_DOWN_CAST<const AnchorSprite>(spr);
		auto real = anchor_spr->QueryAnchor(params.actor.get());
		if (real) {
			SprVisitorParams2 cp = params;
			cp.actor = real->GetSprRaw()->QueryActorRef(params.actor.get());
			return Visit(real->GetSpr(), cp);
		} else {
			return VISIT_OVER;
		}
	} else if (type == SYM_ANIM2) {
		return VISIT_OVER;
	} else if (type == SYM_MASK) {
		int name_id = SprNameMap::Instance()->QueryID("base");
		auto base_spr = spr->FetchChildByName(name_id, params.actor);
		auto c_actor = base_spr->QueryActor(params.actor.get());
		bool visible = c_actor ? c_actor->IsVisible() : base_spr->IsVisible();
		if (!visible) {
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

	bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
	if (editable) 
	{
		m_selected_spr = spr;
		m_selected_params = params;
		m_selected_path = m_curr_path;
		m_finded = true;
		return VISIT_STOP;
	} 
	else if (type == SYM_MASK)
    {
        return VISIT_INTO;
    }
    else
	{
		bool use_new = false;
		if (!m_selected_spr || m_selected_path.IsPartOf(m_curr_path)) {
			use_new = true;
		}
		if (m_selected_spr) {
			if (m_curr_path.IsBatterThan(m_selected_path)) {
				use_new = true;
			}
		}
		if (use_new) {
			m_selected_spr = spr;
			m_selected_params = params;
			m_selected_path = m_curr_path;
			return VISIT_OVER;
		} else {
			return VISIT_OVER;
		}
	}
}

VisitResult PointQueryVisitor::VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams2& params)
{
	bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
	bool visible = params.actor ? params.actor->IsVisible() : spr->IsVisible();
	m_curr_path.Push(spr->GetID(), editable, visible);
	return VISIT_OVER;
}

VisitResult PointQueryVisitor::VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams2& params)
{
	VisitResult ret = VISIT_OVER;

	if (m_selected_path.Empty())
	{
		if (IsPointInScissor(spr, params))
		{
			bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
			m_selected_spr = spr;
			m_selected_params = params;
			m_selected_path = m_curr_path;
			if (editable) {
				m_finded = true;
				ret = VISIT_STOP;
			} else {
				ret = VISIT_OVER;
			}
		}
	}
	else if (m_curr_path.IsPartOf(m_selected_path))
	{
		bool editable = params.actor ? params.actor->IsEditable() : spr->IsEditable();
		if (editable) 
		{
			m_selected_spr = spr;
			m_selected_params = params;
			m_selected_path = m_curr_path;
			m_finded = true;
			ret = VISIT_STOP;
		}
	} 

 	assert(!m_curr_path.Empty());
 	m_curr_path.Pop();

	return ret;
}

ActorConstPtr PointQueryVisitor::GetSelectedActor() const
{
	if (m_selected_spr && m_finded) {
		return m_selected_params.actor;
	} else {
		return nullptr;
	}
}

bool PointQueryVisitor::QuerySprite(const SprConstPtr& spr, const SprVisitorParams2& params) const
{
	sm::rect rect = spr->GetSymbol()->GetBounding(spr.get(), params.actor.get());
	if (rect.Width() == 0 || rect.Height() == 0 || !rect.IsValid()) {
		return false;
	} 
	
	int type = spr->GetSymbol()->Type();
	if (type != SYM_SHAPE) {
		return IsPointInRect(rect, params.mt);
	}

	auto& shape = S2_VI_PTR_DOWN_CAST<const ShapeSymbol>(spr->GetSymbol())->GetShape();
	sm::vec2 pos = params.mt.Inverted() * m_pos;
	if (shape && shape->IsContain(pos)) {
		return true;
	}

	return false;
}

bool PointQueryVisitor::IsPointInScissor(const SprConstPtr& spr, const SprVisitorParams2& params) const
{
	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type != SYM_COMPLEX) {
		return false;
	}

	const sm::rect& rect = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(spr->GetSymbol())->GetScissor();
	if (rect.Width() <= 0 || rect.Height() <= 0) {
		return false;
	}

	return IsPointInRect(rect, params.mt);
}

bool PointQueryVisitor::IsPointInRect(const sm::rect& rect, const S2_MAT& mat) const
{
	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(rect.xmin, rect.ymin);
	vertices[1] = sm::vec2(rect.xmin, rect.ymax);
	vertices[2] = sm::vec2(rect.xmax, rect.ymax);
	vertices[3] = sm::vec2(rect.xmax, rect.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = mat * vertices[i];
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
IsBatterThan(const SprPath& path) const
{
	int path_last_editable_id = -1;
	for (int i = 0, n = path.m_editable.size(); i < n; ++i) {
		if (path.m_editable[i]) {
			path_last_editable_id = path.m_impl.QueryByIndex(i);
		}
	}

	int curr_last_editable_id = -1;
	for (int i = 0, n = m_editable.size(); i < n; ++i) {
		if (m_editable[i]) {
			curr_last_editable_id = m_impl.QueryByIndex(i);
		}
	}
	
	if (path_last_editable_id == -1 && curr_last_editable_id != -1) {
		return true;
	} else if (path_last_editable_id != -1 && curr_last_editable_id == -1) {
		return false;
	} else if (path_last_editable_id == -1 && curr_last_editable_id == -1) {
		return false;
	} else {
		if (path_last_editable_id == curr_last_editable_id) {
			return false;
		}
		for (int i = 0, n = m_editable.size(); i < n; ++i) {
			if (m_editable[i] && m_impl.QueryByIndex(i) == path_last_editable_id) {
				return true;
			}
		}
		return false;
	}
}

bool PointQueryVisitor::SprPath::
Empty() const
{
	return m_impl.Empty();
}

void PointQueryVisitor::SprPath::
Push(int spr_id, bool editable, bool visible)
{
	m_impl.Push(spr_id);
	m_editable.push_back(editable);
	m_visible.push_back(visible);
}

void PointQueryVisitor::SprPath::
Pop()
{
	m_impl.Pop();
	if (!m_editable.empty()) {
		m_editable.pop_back();
	}
	if (!m_visible.empty()) {
		m_visible.pop_back();
	}
}

bool PointQueryVisitor::SprPath::
IsVisible() const
{
	for (int i = 0, n = m_editable.size(); i < n; ++i) {
		if (m_visible[i]) {
			return true;
		}
	}
	return false;
}

}
