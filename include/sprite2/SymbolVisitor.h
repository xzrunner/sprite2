#ifndef _SPRITE2_SYMBOL_VISITOR_H_
#define _SPRITE2_SYMBOL_VISITOR_H_

namespace s2
{

class Sprite;

class SymbolVisitor
{
public:
	virtual ~SymbolVisitor() {}
	virtual void Visit(Sprite* spr) const = 0;
}; // SymbolVisitor

}

#endif // _SPRITE2_SYMBOL_VISITOR_H_
