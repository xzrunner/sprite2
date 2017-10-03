#ifndef _SPRITE2_POLYGON_SHAPE_H_
#define _SPRITE2_POLYGON_SHAPE_H_

#include "PolylineShape.h"

namespace s2
{

class Polygon;

class PolygonShape : public PolylineShape
{
public:
	PolygonShape() = default;
	PolygonShape(const PolygonShape& ps);
	const PolygonShape& operator = (const PolygonShape& ps);
	PolygonShape(const std::vector<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    Shape
	 */
	virtual PolygonShape* Clone() const { return new PolygonShape(*this);  }
	virtual int Type() const { return SHAPE_POLYGON; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Draw(const RenderParams& rp) const;

	void SetPolygon(std::unique_ptr<Polygon> poly);
	const std::unique_ptr<Polygon>& GetPolygon() const { return m_poly; }

protected:
	std::unique_ptr<Polygon> m_poly;
	
}; // PolygonShape

}

#endif // _SPRITE2_POLYGON_SHAPE_H_