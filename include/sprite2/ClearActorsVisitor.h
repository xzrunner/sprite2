#ifndef _SPRITE2_CLEAR_ACTORS_VISITOR_H_
#define _SPRITE2_CLEAR_ACTORS_VISITOR_H_

#include "SprVisitor.h"

namespace s2
{

class ClearActorsVisitor : public SprVisitor
{
public:	
	virtual VisitResult Visit(const Sprite* spr, const sm::mat4* mat);

}; // ClearActorsVisitor

}

#endif // _SPRITE2_CLEAR_ACTORS_VISITOR_H_