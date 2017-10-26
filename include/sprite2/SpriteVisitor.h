#ifndef _SPRITE2_SPRITE_VISITOR_H_
#define _SPRITE2_SPRITE_VISITOR_H_

#include "sprite2/pre_defined.h"
#include S2_MAT_HEADER
#include "sprite2/VisitResult.h"
#include "sprite2/typedef.h"

#include <memory>

namespace s2
{

class Sprite;
class SprVisitorParams;
class SprVisitorParams2;

class SpriteVisitor
{
public:
	SpriteVisitor(bool order = true);
	virtual ~SpriteVisitor() {}

	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams& params) = 0;

	virtual VisitResult VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams& params) { return VISIT_OVER; }
	virtual VisitResult VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams& params) { return VISIT_OVER; }

	bool GetOrder() const { return m_order; }

	static bool VisitChild(SpriteVisitor& visitor, const SprVisitorParams& params, const SprConstPtr& child, VisitResult& ret);

private:
	bool m_order;

}; // SpriteVisitor

class SpriteVisitor2
{
public:
	SpriteVisitor2(bool order = true);
	virtual ~SpriteVisitor2() {}

	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams2& params) = 0;

	virtual VisitResult VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams2& params) { return VISIT_OVER; }
	virtual VisitResult VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams2& params) { return VISIT_OVER; }

	bool GetOrder() const { return m_order; }

	static bool VisitChild(SpriteVisitor2& visitor, const SprVisitorParams2& params, const SprConstPtr& child, VisitResult& ret);

private:
	bool m_order;

}; // SpriteVisitor2

}

#endif // _SPRITE2_SPRITE_VISITOR_H_