#ifndef _SPRITE2_QUERY_AABB_VISITOR_H_
#define _SPRITE2_QUERY_AABB_VISITOR_H_

#include "SpriteVisitor.h"

#include <SM_Rect.h>

namespace s2
{

class QueryAABBVisitor : public SpriteVisitor
{
public:
	QueryAABBVisitor(bool filter_visible = false, bool filter_editable = false, bool filter_scissor = false);

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	const sm::rect& GetAABB() const { return m_aabb; }

private:
	void CombineAABB(const sm::rect& aabb, const sm::mat4& mat);

private:
	bool m_filter_visible;
	bool m_filter_editable;
	bool m_filter_scissor;

	sm::rect m_aabb;

}; // QueryAABBVisitor

}

#endif // _SPRITE2_QUERY_AABB_VISITOR_H_