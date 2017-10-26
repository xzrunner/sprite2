#include "sprite2/ResetActorFlagVisitor.h"
#include "sprite2/Sprite.h"

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