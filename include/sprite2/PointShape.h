#ifndef _SPRITE2_POINT_SHAPE_H_
#define _SPRITE2_POINT_SHAPE_H_

#include "sprite2/Shape.h"
#include "sprite2/ShapeType.h"

namespace s2
{

class PointShape : public VIRTUAL_INHERITANCE Shape
{
public:
	PointShape();
	PointShape(const PointShape& point);
	PointShape(const sm::vec2& pos);

	virtual PointShape* Clone() const override;
	virtual int Type() const override { return SHAPE_POINT; }
	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;
	virtual void Draw(cooking::DisplayList* dlist, const RenderParams& rp) const override;

	const sm::vec2& GetPos() const;
	void SetPos(const sm::vec2& pos);

protected:
	virtual void UpdateBounding() override;

protected:
	sm::vec2 m_pos;

	VI_DUMMY_FUNC

}; // PointShape

}

#endif // _SPRITE2_POINT_SHAPE_H_