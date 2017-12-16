#include "sprite2/BezierShape.h"
#include "sprite2/ShapeConfig.h"
#include "sprite2/RVG.h"
#include "sprite2/RenderParams.h"

#include <SM_Calc.h>

#include <string.h>

namespace s2
{

BezierShape::BezierShape()
{
	memset(m_control_nodes, 0, sizeof(m_control_nodes));
}

BezierShape::BezierShape(const BezierShape& bezier)
	: PolylineShape(bezier)
{
	memcpy(m_control_nodes, bezier.m_control_nodes, sizeof(m_control_nodes));
	UpdatePolyline();
}

BezierShape& BezierShape::operator = (const BezierShape& bezier)
{
	PolylineShape::operator = (bezier);
	memcpy(m_control_nodes, bezier.m_control_nodes, sizeof(m_control_nodes));
	UpdatePolyline();
	return *this;
}

BezierShape::BezierShape(const sm::vec2& start, const sm::vec2& end) 
{
	m_control_nodes[0] = start;
	m_control_nodes[3] = end;

	sm::vec2 mid = (start + end) * 0.5f;
	sm::vec2 offset = (end - start) * 0.5f;
	m_control_nodes[1] = mid + sm::rotate_vector_right_angle(offset, true);
	m_control_nodes[2] = mid + sm::rotate_vector_right_angle(offset, false);

	UpdatePolyline();
}

BezierShape* BezierShape::Clone() const
{
	return new BezierShape(*this);
}

bool BezierShape::IsContain(const sm::vec2& pos) const
{
	bool ret = false;
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (sm::dis_pos_to_pos(pos, m_control_nodes[i]) < SHAPE_NODE_RADIUS) {
			ret = true;
			break;
		}
	}
	return ret;
}

void BezierShape::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	PolylineShape::Draw(dlist, rp);
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		sm::vec2 pos = rp.mt * m_control_nodes[i];
		RVG::Rect(dlist, pos, SHAPE_NODE_RADIUS, SHAPE_NODE_RADIUS, false);
	}
}

void BezierShape::UpdatePolyline()
{
	const int num = std::max(20, (int)(sm::dis_pos_to_pos(m_control_nodes[0], m_control_nodes[3]) / 10));
	float dt = 1.0f / (num - 1);
	CU_VEC<sm::vec2> vertices;
	vertices.resize(num);
	for (int i = 0; i < num; ++i) {
		vertices[i] = PointOnCubicBezier(i * dt);
	}

	m_vertices = vertices;
	UpdateBounding();
}

sm::vec2 BezierShape::PointOnCubicBezier(float t)
{
	float ax, bx, cx;
	float ay, by, cy;
	float squared, cubed;
	sm::vec2 result;

	cx = 3.0f * (m_control_nodes[1].x - m_control_nodes[0].x);
	bx = 3.0f * (m_control_nodes[2].x - m_control_nodes[1].x) - cx;
	ax = m_control_nodes[3].x - m_control_nodes[0].x - cx - bx;

	cy = 3.0f * (m_control_nodes[1].y - m_control_nodes[0].y);
	by = 3.0f * (m_control_nodes[2].y - m_control_nodes[1].y) - cy;
	ay = m_control_nodes[3].y - m_control_nodes[0].y - cy - by;

	squared = t * t;
	cubed = squared * t;

	result.x = (ax * cubed) + (bx * squared) + (cx * t) + m_control_nodes[0].x;
	result.y = (ay * cubed) + (by * squared) + (cy * t) + m_control_nodes[0].y;

	return result;
}

}