#include "PolygonShape.h"
#include "Polygon.h"

#include <SM_Test.h>

namespace s2
{

PolygonShape::PolygonShape()
	: m_poly(NULL)
{
}

PolygonShape::PolygonShape(const PolygonShape& poly)
	: PolylineShape(poly)
	, m_poly(NULL)
{
	cu::RefCountObjAssign(m_poly, poly.m_poly);
}

PolygonShape& PolygonShape::operator = (const PolygonShape& poly)
{
	PolylineShape::operator = (poly);

	cu::RefCountObjAssign(m_poly, poly.m_poly);
	return *this;
}

PolygonShape::PolygonShape(const std::vector<sm::vec2>& vertices)
	: PolylineShape(vertices, true)
	, m_poly(NULL)
{
}

PolygonShape::~PolygonShape()
{
	if (m_poly) {
		m_poly->RemoveReference();
	}
}

PolygonShape* PolygonShape::Clone() const
{
	return new PolygonShape(*this);
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

void PolygonShape::DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	// todo
}

void PolygonShape::SetPolygon(Polygon* poly)
{
	cu::RefCountObjAssign(m_poly, poly);

	m_bounding.MakeEmpty();
	if (poly) {
		const std::vector<sm::vec2>& tris = poly->GetTriangles();
		for (int i = 0, n = tris.size(); i < n; ++i) {
			m_bounding.Combine(tris[i]);
		}
	}
}

}