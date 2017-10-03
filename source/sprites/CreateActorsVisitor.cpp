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

VisitResult CreateActorsVisitor::Visit(const SprConstPtr& spr, const SprVisitorParams& params)
{
	return VISIT_INTO;
}

VisitResult CreateActorsVisitor::VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams& params)
{
	if (spr->IsNeedActor() || spr->IsNeedActorForChild()) 
	{
		auto child = ActorFactory::Create(m_path.top(), spr);
		const_cast<SprVisitorParams&>(params).actor = child;
		m_path.push(child);
	}
	return VISIT_OVER;
}

VisitResult CreateActorsVisitor::VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams& params)
{
	if (spr->IsNeedActor() || spr->IsNeedActorForChild()) {
		m_path.pop();
	}
	return VISIT_OVER;
}

}