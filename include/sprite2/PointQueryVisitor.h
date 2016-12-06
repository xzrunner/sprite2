#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "SprVisitor.h"

#include <SM_Vector.h>

namespace s2
{

class PointQueryVisitor : public SprVisitor
{
public:
	PointQueryVisitor(const sm::vec2& pos);
	virtual ~PointQueryVisitor();

	virtual VisitResult Visit(const Sprite* spr);

	const Sprite* GetResult() const { return m_result; }
	
private:
	sm::vec2 m_pos;

	const Sprite* m_result;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_