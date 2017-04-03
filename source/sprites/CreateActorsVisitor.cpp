#include "CreateActorsVisitor.h"
#include "S2_Sprite.h"
#include "ActorFactory.h"
#include "SprVisitorParams.h"

namespace s2
{

CreateActorsVisitor::CreateActorsVisitor()
	: m_need_actor(false)
{
}

VisitResult CreateActorsVisitor::Visit(const Sprite* spr, SprVisitorParams& params)
{
	return VISIT_INTO;
}

VisitResult CreateActorsVisitor::VisitChildrenBegin(const Sprite* spr, SprVisitorParams& params)
{
	m_need_actor = spr->IsNeedActor() || spr->IsNeedActorForChild();
	return VISIT_OVER;
}

VisitResult CreateActorsVisitor::VisitChildrenEnd(const Sprite* spr, SprVisitorParams& params)
{
	if (!m_need_actor) {
		return VISIT_OVER;
	}

	ActorFactory::Instance()->Create(params.actor, spr);

	return VISIT_OVER;
}

}