#ifndef _SPRITE2_RECT_SHAPE_H_
#define _SPRITE2_RECT_SHAPE_H_

#include "sprite2/Shape.h"
#include "sprite2/ShapeType.h"

namespace s2
{

class RectShape : public VIRTUAL_INHERITANCE Shape
{
public:
	RectShape() {}
	RectShape(const RectShape& rect);
	RectShape(const sm::rect& r);
	
	virtual RectShape* Clone() const override;
	virtual int Type() const override { return SHAPE_RECT; }
	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;
	virtual void Draw(cooking::DisplayList* dlist, const RenderParams& rp) const override;

	const sm::rect& GetRect() const { return m_bounding; }
	void SetRect(const sm::rect& r) { m_bounding = r; }

protected:
	virtual void UpdateBounding() override {}

	VI_DUMMY_FUNC

}; // RectShape

}

#endif // _SPRITE2_RECT_SHAPE_H_