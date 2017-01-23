#ifndef _SPRITE2_SYM_VISITOR_H_
#define _SPRITE2_SYM_VISITOR_H_

namespace s2
{

class Sprite;

class SymVisitor
{
public:
	virtual ~SymVisitor() {}
	virtual void Visit(Sprite* spr) const = 0;
}; // SymVisitor

}

#endif // _SPRITE2_SYM_VISITOR_H_
