#ifndef _SPRITE2_SPR_VISITOR_H_
#define _SPRITE2_SPR_VISITOR_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;

enum VisitResult
{
	VISIT_IGNORE = 0,
	VISIT_INTO,
	VISIT_STOP,
	VISIT_CONTINUE,
};

class SprVisitor
{
public:
	SprVisitor(bool order = true);
	virtual ~SprVisitor() {}

	virtual VisitResult Visit(const Sprite* spr, const sm::mat4* mat) = 0;

	virtual void VisitChildrenBegin(const Sprite* spr) {}
	virtual void VisitChildrenEnd(const Sprite* spr) {}

	bool GetOrder() const { return m_order; }

private:
	bool m_order;

}; // SprVisitor

}

#endif // _SPRITE2_SPR_VISITOR_H_