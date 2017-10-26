#ifndef _SPRITE2_POLYLINE_SHAPE_H_
#define _SPRITE2_POLYLINE_SHAPE_H_

#include "sprite2/Shape.h"
#include "sprite2/ShapeType.h"

#include <cu/cu_stl.h>

namespace s2
{

class PolylineShape : public VIRTUAL_INHERITANCE Shape
{
public:
	PolylineShape();
	PolylineShape(const PolylineShape& polyline);
	PolylineShape(const CU_VEC<sm::vec2>& vertices, bool closed = false);

	/**
	 *  @interface
	 *    Shape
	 */
	virtual PolylineShape* Clone() const { return new PolylineShape(*this);  }
	virtual int Type() const { return SHAPE_POLYLINE; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Draw(const RenderParams& rp) const;

	const CU_VEC<sm::vec2>& GetVertices() const { return m_vertices; }
	void SetVertices(const CU_VEC<sm::vec2>& vertices);

protected:
	virtual void UpdateBounding();

protected:
	CU_VEC<sm::vec2> m_vertices;
	bool m_closed;

	VI_DUMMY_FUNC

}; // PolylineShape

}

#endif // _SPRITE2_POLYLINE_SHAPE_H_