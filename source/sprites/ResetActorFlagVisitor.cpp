#include "ResetActorFlagVisitor.h"
#include "S2_Sprite.h"

namespace s2
{

ResetActorFlagVisitor::ResetActorFlagVisitor()
	: m_need_actor(false)
{
}

VisitResult ResetActorFlagVisitor::Visit(const SprConstPtr& spr, const SprVisitorParams& params)
{
	if (spr->IsNeedActor()) {
		m_need_actor = true;
	}
	return VISIT_INTO;
}

VisitResult ResetActorFlagVisitor::VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams& params)
{
	if (m_need_actor) {
		spr->SetNeedActorForChild(true);
	}
	return VISIT_OVER;
}

}