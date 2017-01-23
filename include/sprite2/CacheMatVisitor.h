#ifndef _SPRITE2_CACHE_MAT_VISITOR_H_
#define _SPRITE2_CACHE_MAT_VISITOR_H_

#include "SymVisitor.h"

namespace s2
{

class CacheMatVisitor : public SymVisitor
{
public:
	virtual void Visit(Sprite* spr) const;
}; // CacheMatVisitor

}

#endif // _SPRITE2_CACHE_MAT_VISITOR_H_