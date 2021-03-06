#include "sprite2/CosineShape.h"
#include "sprite2/ShapeVertices.h"

#include <SM_CosineSmooth.h>
#include <SM_DouglasPeucker.h>

namespace s2
{

static const float SAMPLING_WIDTH = 10.0f;

CosineShape::CosineShape()
{
}

CosineShape::CosineShape(const CosineShape& cosine)
	: PolylineShape(cosine)
	, m_mid_points(cosine.m_mid_points)
{
	UpdatePolyline();
}

CosineShape::CosineShape(const CU_VEC<sm::vec2>& vertices)
	: PolylineShape(vertices) 
{
	UpdatePolyline();
}

CosineShape* CosineShape::Clone() const
{
	return new CosineShape(*this);
}

void CosineShape::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	ShapeVertices::Draw(dlist, m_mid_points, m_closed, rp);
}

void CosineShape::UpdatePolyline()
{
	if (m_vertices.size() <= 1) {
		return;
	}

	m_mid_points.clear();

	CU_VEC<sm::vec2> smooth;
	sm::cosine_smooth(m_vertices, SAMPLING_WIDTH, smooth);

	sm::douglas_peucker(smooth, 0.75f, m_mid_points);
}

}