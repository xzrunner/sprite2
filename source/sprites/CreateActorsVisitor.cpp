#include "CreateActorsVisitor.h"
#include "S2_Sprite.h"
#include "ActorFactory.h"
#include "SprVisitorParams.h"

namespace s2
{

CreateActorsVisitor::CreateActorsVisitor()
	: m_need_actor(false)
{
	m_path.push(nullptr);
}

VisitResult CreateActorsVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	return VISIT_INTO;
}

VisitResult CreateActorsVisitor::VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params)
{
	if (spr->IsNeedActor() || spr->IsNeedActorForChild()) 
	{
		const Actor* parent = m_path.top();
		const Actor* child = ActorFactory::Instance()->Create(parent, spr);
		const_cast<SprVisitorParams&>(params).actor = child;
		m_path.push(child);
	}
	return VISIT_OVER;
}

VisitResult CreateActorsVisitor::VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params)
{
	if (spr->IsNeedActor() || spr->IsNeedActorForChild()) {
		m_path.pop();
	}
	return VISIT_OVER;
}

}