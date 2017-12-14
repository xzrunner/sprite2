#ifndef _SPRITE2_POLYGON_SHAPE_H_
#define _SPRITE2_POLYGON_SHAPE_H_

#include "sprite2/PolylineShape.h"
#include "sprite2/Polygon.h"

namespace s2
{

class PolygonShape : public PolylineShape
{
public:
	PolygonShape() = default;
	PolygonShape(const PolygonShape& ps);
	const PolygonShape& operator = (const PolygonShape& ps);
	PolygonShape(const CU_VEC<sm::vec2>& vertices);
	virtual ~PolygonShape();

	/**
	 *  @interface
	 *    Shape
	 */
	virtual PolygonShape* Clone() const override { return new PolygonShape(*this);  }
	virtual int Type() const override { return SHAPE_POLYGON; }
	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;
	virtual void Draw(cooking::DisplayList* dlist, const RenderParams& rp) const override;

	void SetPolygon(std::unique_ptr<Polygon> poly);
	const std::unique_ptr<Polygon>& GetPolygon() const { return m_poly; }

protected:
	std::unique_ptr<Polygon> m_poly = nullptr;
	
}; // PolygonShape

}

#endif // _SPRITE2_POLYGON_SHAPE_H_