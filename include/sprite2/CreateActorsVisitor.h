#ifndef _SPRITE2_CREATE_ACTORS_VISITOR_H_
#define _SPRITE2_CREATE_ACTORS_VISITOR_H_

#include "SpriteVisitor.h"

#include <stack>

namespace s2
{

class Actor;

class CreateActorsVisitor : public SpriteVisitor
{
public:
	CreateActorsVisitor();

	virtual VisitResult Visit(const Sprite* spr, SprVisitorParams& params);

	virtual VisitResult VisitChildrenBegin(const Sprite* spr, SprVisitorParams& params);
	virtual VisitResult VisitChildrenEnd(const Sprite* spr, SprVisitorParams& params);

private:
	bool m_need_actor;

	std::stack<const Actor*> m_path;

}; // CreateActorsVisitor

}

#endif // _SPRITE2_CREATE_ACTORS_VISITOR_H_