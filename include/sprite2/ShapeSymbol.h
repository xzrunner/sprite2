#ifndef _SPRITE2_SHAPE_SYMBOL_H_
#define _SPRITE2_SHAPE_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/Shape.h"

#include <stdint.h>

namespace s2
{

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN

#ifndef S2_SHAPE_SHARED_PTR
	void SetShape(std::unique_ptr<Shape> shape) { m_shape = std::move(shape); }
#else
	void SetShape(const std::shared_ptr<Shape>& shape) { m_shape = shape; }
#endif // S2_SHAPE_SHARED_PTR
	const auto& GetShape() const { return m_shape; }
	auto& GetShape() { return m_shape; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
#ifndef S2_SHAPE_SHARED_PTR
	std::unique_ptr<Shape> m_shape;
#else
	std::shared_ptr<Shape> m_shape;
#endif // S2_SHAPE_SHARED_PTR

}; // ShapeSymbol

}

#endif // _SPRITE2_SHAPE_SYMBOL_H_