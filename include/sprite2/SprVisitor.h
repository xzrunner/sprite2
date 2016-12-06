#ifndef _SPRITE2_SPR_VISITOR_H_
#define _SPRITE2_SPR_VISITOR_H_

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
	virtual VisitResult Visit(const Sprite* spr) = 0;
	virtual ~SprVisitor() {}
}; // SprVisitor

}

#endif // _SPRITE2_SPR_VISITOR_H_