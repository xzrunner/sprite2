#ifndef _SPRITE2_SHAPE_H_
#define _SPRITE2_SHAPE_H_

#include "pre_defined.h"

#include <cu/cu_macro.h>
#include <SM_Vector.h>
#include <SM_Rect.h>
#include S2_MAT_HEADER

#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class RenderParams;

class Shape
{
public:
	Shape() {}
	Shape(const Shape& shape) : m_bounding(shape.m_bounding) {}
	virtual ~Shape() {}

	virtual Shape* Clone() const = 0;

	virtual int Type() const = 0;

	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void Draw(const RenderParams& rp) const = 0;

	const sm::rect& GetBounding() const {
		return m_bounding;
	}

protected:
	virtual void UpdateBounding() = 0;

protected:
	sm::rect m_bounding;

}; // Shape

}

#endif // _SPRITE2_SHAPE_H_