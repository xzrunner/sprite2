#include "ShapeSymbol.h"
#include "SymType.h"
#include "Shape.h"
#include "RenderParams.h"
#include "S2_Sprite.h"

namespace s2
{

ShapeSymbol::ShapeSymbol()
	: m_shape(NULL)
{
}

ShapeSymbol::ShapeSymbol(uint32_t id)
	: Symbol(id)
	, m_shape(NULL)
{
}

ShapeSymbol::~ShapeSymbol()
{
	if (m_shape) {
		m_shape->RemoveReference();
	}
}

int ShapeSymbol::Type() const 
{ 
	return SYM_SHAPE; 
}

void ShapeSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_shape) {
		return;
	}

	if (spr) {
		sm::mat4 mt = spr->GetTransMatrix() * params.mt;
		RenderColor color = spr->GetColor() * params.color;
		m_shape->Draw(mt, color);
	}
}

sm::rect ShapeSymbol::GetBounding(const Sprite* spr) const
{
	if (m_shape) {
		return m_shape->GetBounding();
	} else {
		return sm::rect(0, 0);
	}
}

void ShapeSymbol::SetShape(Shape* shape)
{
	cu::RefCountObjAssign(m_shape, shape);
}

}