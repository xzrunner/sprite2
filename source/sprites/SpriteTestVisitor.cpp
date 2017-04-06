#include "SpriteTestVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "BoundingBox.h"
#include "SprVisitorParams.h"
// #include "ActorLUT.h"
#include "QueryAABBVisitor.h"
#include "AnchorSprite.h"


#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

SpriteTestVisitor::SpriteTestVisitor(const sm::vec2& pos)
	: SpriteVisitor(false)
	, m_pos(pos)
{
}

SpriteTestVisitor::~SpriteTestVisitor()
{
}

VisitResult SpriteTestVisitor::Visit(const Sprite* spr, SprVisitorParams& params)
{
	if (!spr->IsVisible()) {
		return VISIT_OVER;
	}

	const Actor* actor = params.actor;
	if (actor && !actor->IsVisible()) {
		return VISIT_OVER;
	}

	bool editable = (actor ? actor->IsEditable() : spr->IsEditable());

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

	// branch node
	if (type == SYM_ANIMATION) {
		return VISIT_INTO;
	}
	if (type == SYM_COMPLEX) {
		if (QueryScissor(spr, params, NULL)) {
			return VISIT_INTO;
		} else {
			return VISIT_OVER;
		}
	}

	// leaf node
	else {
		if (QuerySprite(spr, params)) {
			if (editable) {
				m_selected_params = params;
				return VISIT_STOP;
			} else {
				params.selected = true;
				return VISIT_OVER;
			}
		} else {
			return VISIT_OVER;
		}
	}
}

VisitResult SpriteTestVisitor::VisitChildrenBegin(const Sprite* spr, SprVisitorParams& params)
{
	return VISIT_OVER;
}

VisitResult SpriteTestVisitor::VisitChildrenEnd(const Sprite* spr, SprVisitorParams& params)
{
	const Actor* actor = params.actor;
	bool editable = (actor ? actor->IsEditable() : spr->IsEditable());

	if (!params.selected) {
		bool has_scissor;
		QueryScissor(spr, params, &has_scissor);
		if (has_scissor) {
			params.selected = true;
		}
	}

	if (params.selected) {
		if (editable) {
			m_selected_params = params;
			return VISIT_STOP;
		}
	}
	return VISIT_OVER;
}

const Actor* SpriteTestVisitor::GetSelectedActor() const
{
	return m_selected_params.actor;
}

bool SpriteTestVisitor::QueryScissor(const Sprite* spr, const SprVisitorParams& params, bool* out_has_scissor) const
{
	if(out_has_scissor) {
		*out_has_scissor = false;
	}

	const sm::rect& scissor = spr->GetSymbol()->GetScissor();
	if(!scissor.IsValid()) {
		return true;
	}

	sm::vec2 scissor_sz = scissor.Size();
	bool has_scissor = scissor_sz.x > 0 && scissor_sz.y > 0;
	if (!has_scissor) {
		return true;
	}

	if(out_has_scissor) {
		*out_has_scissor = true;
	}

	sm::vec2 min = params.mt * sm::vec2(scissor.xmin, scissor.ymin),
		     max = params.mt * sm::vec2(scissor.xmax, scissor.ymax);
	if (min.x > max.x) {
		std::swap(min.x, max.x);
	}
	if (min.y > max.y) {
		std::swap(min.y, max.y);
	}

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(min.x, min.y);
	vertices[1] = sm::vec2(min.x, max.y);
	vertices[2] = sm::vec2(max.x, max.y);
	vertices[3] = sm::vec2(max.x, min.y);
	return sm::is_point_in_convex(m_pos, vertices, 4);
}

bool SpriteTestVisitor::QuerySprite(const Sprite* spr, const SprVisitorParams& params) const
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

}