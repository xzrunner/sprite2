#ifndef _SPRITE2_QUERY_LOADED_VISITOR_H_
#define _SPRITE2_QUERY_LOADED_VISITOR_H_

#include "SpriteVisitor.h"

namespace s2
{

class QueryLoadedVisitor : public SpriteVisitor
{
public:
	QueryLoadedVisitor();

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	bool IsLoaded() const { return m_loaded; }

private:
	bool m_loaded;

}; // QueryLoadedVisitor

}

#endif // _SPRITE2_QUERY_LOADED_VISITOR_H_