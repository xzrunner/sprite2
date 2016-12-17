#ifndef _SPRITE2_FIX_ACTOR_PATH_VISITOR_H_
#define _SPRITE2_FIX_ACTOR_PATH_VISITOR_H_

#include "SprVisitor.h"
#include "SprTreePath.h"

namespace s2
{

class FixActorPathVisitor : public SprVisitor
{
public:	
	FixActorPathVisitor(const SprTreePath& path);

	virtual VisitResult Visit(const Sprite* spr, const sm::mat4* mat);

	virtual void VisitChildrenBegin(const Sprite* spr);
	virtual void VisitChildrenEnd(const Sprite* spr);

private:
	SprTreePath m_path;

}; // FixActorPathVisitor

}

#endif // _SPRITE2_FIX_ACTOR_PATH_VISITOR_H_