#include "QueryAABBVisitor.h"
#include "S2_Sprite.h"
#include "SprTreePath.h"
#include "SprVisitorParams.h"
#include "ActorLUT.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "S2_Symbol.h"
#include "ComplexSymbol.h"

namespace s2
{

QueryAABBVisitor::QueryAABBVisitor(bool filter_visible, bool filter_editable, bool filter_scissor)
	: m_filter_visible(filter_visible)
	, m_filter_editable(filter_editable)
	, m_filter_scissor(filter_scissor)
{
}

VisitResult QueryAABBVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	if (spr->IsHasProxy()) {
		SprTreePath path = params.path;
		path.Pop();
		const Sprite* proxy = spr->GetProxy(path);
		if (proxy) {
			spr = proxy;
		}
	}

	bool visible = spr->IsVisible();
	bool editable = spr->IsEditable();
	if (spr->HaveActor()) {
		Actor* actor = ActorLUT::Instance()->Query(params.path);
		if (actor) {
			visible = actor->IsVisible();
			editable = actor->IsEditable();
		}
	}
	if (m_filter_visible && !visible) {
		return VISIT_OVER;
	}
	if (m_filter_editable && !editable) {
		return VISIT_OVER;
	}

	VisitResult ret = VISIT_OVER;

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	switch (type)
	{
	case SYM_INVALID: case SYM_UNKNOWN:
		ret = VISIT_OVER;
		break;
	case SYM_COMPLEX:
		{
			const s2::ComplexSymbol* complex_sym = VI_DOWNCASTING<const s2::ComplexSymbol*>(spr->GetSymbol());
			const sm::rect& scissor = complex_sym->GetScissor();
			if (m_filter_scissor && scissor.Width() > 0 && scissor.Height() > 0) {
				CombineAABB(scissor, params.mt);
				ret = VISIT_OVER;
			} else {
				ret = VISIT_INTO;
			}
		}
		break;
	case SYM_ANIMATION:
		ret = VISIT_INTO;
		break;
	default:
		CombineAABB(spr->GetSymbol()->GetBounding(spr), params.mt);
		break;
	}

	return ret;
}

void QueryAABBVisitor::CombineAABB(const sm::rect& aabb, const sm::mat4& mat)
{
	if (aabb.Width() == 0 || aabb.Height() == 0) {
		return;
	}
	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(aabb.xmin, aabb.ymin);
	vertices[1] = sm::vec2(aabb.xmin, aabb.ymax);
	vertices[2] = sm::vec2(aabb.xmax, aabb.ymax);
	vertices[3] = sm::vec2(aabb.xmax, aabb.ymin);
	for (int i = 0; i < 4; ++i) {
		m_aabb.Combine(mat * vertices[i]);
	}
}

}