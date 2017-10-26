#ifndef _SPRITE2_RESET_ACTOR_FLAG_VISITOR_H_
#define _SPRITE2_RESET_ACTOR_FLAG_VISITOR_H_

#include "sprite2/SpriteVisitor.h"

namespace s2
{

class ResetActorFlagVisitor : public SpriteVisitor
{
public:
	ResetActorFlagVisitor();

	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams& params);

	virtual VisitResult VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams& params);

private:
	bool m_need_actor;

}; // ResetActorFlagVisitor

}

#endif // _SPRITE2_RESET_ACTOR_FLAG_VISITOR_H_