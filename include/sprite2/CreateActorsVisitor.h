#ifndef _SPRITE2_CREATE_ACTORS_VISITOR_H_
#define _SPRITE2_CREATE_ACTORS_VISITOR_H_

#include "sprite2/SpriteVisitor.h"

#include <stack>

namespace s2
{

class Actor;

class CreateActorsVisitor : public SpriteVisitor
{
public:
	CreateActorsVisitor();

	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams& params);

	virtual VisitResult VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams& params);
	virtual VisitResult VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams& params);

private:
	bool m_need_actor;

	std::stack<ActorConstPtr> m_path;

}; // CreateActorsVisitor

}

#endif // _SPRITE2_CREATE_ACTORS_VISITOR_H_