#ifndef _SPRITE2_POINT_SHAPE_H_
#define _SPRITE2_POINT_SHAPE_H_

#include "Shape.h"
#include "ShapeType.h"

namespace s2
{

class PointShape : public VIRTUAL_INHERITANCE Shape
{
public:
	PointShape();
	PointShape(const PointShape& point);
	PointShape(const sm::vec2& pos);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual PointShape* Clone() const;	

	/**
	 *  @interface
	 *    Shape
	 */
	virtual int Type() const { return SHAPE_POINT; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Draw(const RenderParams& rp) const;

	const sm::vec2& GetPos() const;
	void SetPos(const sm::vec2& pos);

protected:
	virtual void UpdateBounding();

protected:
	sm::vec2 m_pos;

	VI_DUMMY_FUNC

}; // PointShape

}

#endif // _SPRITE2_POINT_SHAPE_H_