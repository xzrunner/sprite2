#include "CreateActorsVisitor.h"
#include "S2_Sprite.h"
#include "ActorFactory.h"
#include "SprVisitorParams.h"

namespace s2
{

CreateActorsVisitor::CreateActorsVisitor()
	: m_need_actor(false)
{
	m_path.push(NULL);
}

VisitResult CreateActorsVisitor::Visit(const Sprite* spr, SprVisitorParams& params)
{
	return VISIT_INTO;
}

VisitResult CreateActorsVisitor::VisitChildrenBegin(const Sprite* spr, SprVisitorParams& params)
{
	if (spr->IsNeedActor() || spr->IsNeedActorForChild()) 
	{
		const Actor* parent = m_path.top();
		const Actor* child = ActorFactory::Instance()->Create(parent, spr);
		m_path.push(child);
	}
	return VISIT_OVER;
}

VisitResult CreateActorsVisitor::VisitChildrenEnd(const Sprite* spr, SprVisitorParams& params)
{
	if (spr->IsNeedActor() || spr->IsNeedActorForChild()) {
		m_path.pop();
	}
	return VISIT_OVER;
}

}