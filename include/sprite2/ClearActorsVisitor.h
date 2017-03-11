#ifndef _SPRITE2_CLEAR_ACTORS_VISITOR_H_
#define _SPRITE2_CLEAR_ACTORS_VISITOR_H_

#include "SpriteVisitor.h"

namespace s2
{

class ClearActorsVisitor : public SpriteVisitor
{
public:	
	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

}; // ClearActorsVisitor

}

#endif // _SPRITE2_CLEAR_ACTORS_VISITOR_H_