#ifndef _SPRITE2_CACHE_MAT_VISITOR_H_
#define _SPRITE2_CACHE_MAT_VISITOR_H_

#include "sprite2/SymbolVisitor.h"

namespace s2
{

class CacheMatVisitor : public SymbolVisitor
{
public:
	virtual void Visit(const SprPtr& spr) const override;
}; // CacheMatVisitor

}

#endif // _SPRITE2_CACHE_MAT_VISITOR_H_