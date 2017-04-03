#ifndef _SPRITE2_RESET_ACTOR_FLAG_VISITOR_H_
#define _SPRITE2_RESET_ACTOR_FLAG_VISITOR_H_

#include "SpriteVisitor.h"

namespace s2
{

class ResetActorFlagVisitor : public SpriteVisitor
{
public:
	ResetActorFlagVisitor();

	virtual VisitResult Visit(const Sprite* spr, SprVisitorParams& params);

	virtual VisitResult VisitChildrenEnd(const Sprite* spr, SprVisitorParams& params);

private:
	bool m_need_actor;

}; // ResetActorFlagVisitor

}

#endif // _SPRITE2_RESET_ACTOR_FLAG_VISITOR_H_