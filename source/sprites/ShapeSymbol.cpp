#include "ShapeSymbol.h"
#include "SymType.h"
#include "Shape.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

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

RenderReturn ShapeSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_shape || !spr) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (DrawNode::Prepare(rp, spr, *rp_child)) {
		// todo: shape's render ret
		m_shape->Draw(*rp_child);
		ret = RENDER_OK;
	} else {
		ret = RENDER_INVISIBLE;
	}
	RenderParamsPool::Instance()->Push(rp_child); 
	return ret;
}

void ShapeSymbol::SetShape(Shape* shape)
{
	cu::RefCountObjAssign(m_shape, shape);
}

sm::rect ShapeSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (m_shape) {
		return m_shape->GetBounding();
	} else {
		return sm::rect(0, 0);
	}
}

}