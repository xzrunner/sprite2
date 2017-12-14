#ifndef _SPRITE2_CIRCLE_SHAPE_H_
#define _SPRITE2_CIRCLE_SHAPE_H_

#include "sprite2/Shape.h"
#include "sprite2/ShapeType.h"

namespace s2
{

class CircleShape : public VIRTUAL_INHERITANCE Shape
{
public:
	CircleShape();
	CircleShape(const CircleShape& circle);
	CircleShape(const sm::vec2& center, float radius);

	virtual CircleShape* Clone() const override;
	virtual int Type() const override { return SHAPE_CIRCLE; }
	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;
	virtual void Draw(cooking::DisplayList* dlist, const RenderParams& rp) const override;

	const sm::vec2& GetCenter() const;
	void SetCenter(const sm::vec2& center);

	float GetRadius() const;
	void SetRadius(float radius);

protected:
	virtual void UpdateBounding() override;

protected:
	sm::vec2 m_center;
	float m_radius;

	VI_DUMMY_FUNC

}; // CircleShape

}

#endif // _SPRITE2_CIRCLE_SHAPE_H_	