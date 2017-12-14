#include "sprite2/PolygonShape.h"
#include "sprite2/Polygon.h"

#include <SM_Test.h>

namespace s2
{

PolygonShape::PolygonShape(const PolygonShape& ps)
{
	this->operator = (ps);
}

const PolygonShape& PolygonShape::operator = (const PolygonShape& ps)
{
	if (ps.m_poly) {
		m_poly = ps.m_poly->Clone();
	}
	return *this;
}

PolygonShape::PolygonShape(const CU_VEC<sm::vec2>& vertices)
	: PolylineShape(vertices, true)
{
}

PolygonShape::~PolygonShape() = default;

bool PolygonShape::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_area(pos, m_vertices);
}

bool PolygonShape::IsIntersect(const sm::rect& rect) const
{
	return sm::is_rect_intersect_polygon(rect, m_vertices);
}

void PolygonShape::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	if (m_poly) {
		m_poly->Draw(dlist, rp);
	} else {
		PolylineShape::Draw(dlist, rp);
	}
}

void PolygonShape::SetPolygon(std::unique_ptr<Polygon> poly)
{
	m_poly = std::move(poly);

	m_bounding.MakeEmpty();
	if (poly) {
		const CU_VEC<sm::vec2>& tris = poly->GetTriangles();
		for (int i = 0, n = tris.size(); i < n; ++i) {
			m_bounding.Combine(tris[i]);
		}
	}
}

}