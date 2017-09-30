#ifndef _SPRITE2_SHAPE_SYMBOL_H_
#define _SPRITE2_SHAPE_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Shape;

class ShapeSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ShapeSymbol();
	ShapeSymbol(uint32_t id);
	virtual ~ShapeSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;

	void SetShape(Shape* shape);
	const Shape* GetShape() const { return m_shape; }
	
protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	Shape* m_shape;

}; // ShapeSymbol

}

#endif // _SPRITE2_SHAPE_SYMBOL_H_