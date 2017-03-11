#ifndef _SPRITE2_QUERY_AABB_VISITOR_H_
#define _SPRITE2_QUERY_AABB_VISITOR_H_

#include "SpriteVisitor.h"

#include <SM_Rect.h>

namespace s2
{

class QueryAABBVisitor : public SpriteVisitor
{
public:
	QueryAABBVisitor(bool use_visible = true, bool use_editable = true, bool use_scissor = true);

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	const sm::rect& GetAABB() const { return m_aabb; }

private:
	void CombineAABB(const sm::rect& aabb, const sm::mat4& mat);

private:
	bool m_use_visible;
	bool m_use_editable;
	bool m_use_scissor;

	sm::rect m_aabb;

}; // QueryAABBVisitor

}

#endif // _SPRITE2_QUERY_AABB_VISITOR_H_