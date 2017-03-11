#ifndef _SPRITE2_SPRITE_VISITOR_H_
#define _SPRITE2_SPRITE_VISITOR_H_

#include "pre_defined.h"

#include S2_MAT_HEADER

#include "VisitResult.h"

namespace s2
{

class Sprite;
class SprVisitorParams;

class SpriteVisitor
{
public:
	SpriteVisitor(bool order = true);
	virtual ~SpriteVisitor() {}

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params) = 0;

	virtual VisitResult VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params) { return VISIT_OVER; }
	virtual VisitResult VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params) { return VISIT_OVER; }

	bool GetOrder() const { return m_order; }

	static bool VisitChild(SpriteVisitor& visitor, const SprVisitorParams& params, Sprite* child, VisitResult& ret);

private:
	bool m_order;

}; // SpriteVisitor

}

#endif // _SPRITE2_SPRITE_VISITOR_H_