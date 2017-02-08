#ifndef _SPRITE2_FIX_ACTOR_PATH_VISITOR_H_
#define _SPRITE2_FIX_ACTOR_PATH_VISITOR_H_

#include "SprVisitor.h"
#include "SprTreePath.h"

namespace s2
{

class FixActorPathVisitor : public SprVisitor
{
public:	
	FixActorPathVisitor(const SprTreePath& parent_path);

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	virtual void VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params);
	virtual void VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params);

private:
	SprTreePath m_parent_path;

}; // FixActorPathVisitor

}

#endif // _SPRITE2_FIX_ACTOR_PATH_VISITOR_H_