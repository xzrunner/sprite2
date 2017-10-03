#include "PolygonShape.h"
#include "Polygon.h"

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

PolygonShape::PolygonShape(const std::vector<sm::vec2>& vertices)
	: PolylineShape(vertices, true)
{
}

bool PolygonShape::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_area(pos, m_vertices);
}

bool PolygonShape::IsIntersect(const sm::rect& rect) const
{
	return sm::is_rect_intersect_polygon(rect, m_vertices);
}

void PolygonShape::Draw(const RenderParams& rp) const
{
	if (m_poly) {
		m_poly->Draw(rp);
	} else {
		PolylineShape::Draw(rp);
	}
}

void PolygonShape::SetPolygon(std::unique_ptr<Polygon> poly)
{
	m_poly = std::move(poly);

	m_bounding.MakeEmpty();
	if (poly) {
		const std::vector<sm::vec2>& tris = poly->GetTriangles();
		for (int i = 0, n = tris.size(); i < n; ++i) {
			m_bounding.Combine(tris[i]);
		}
	}
}

}