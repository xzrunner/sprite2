#include "CircleShape.h"
#include "ShapeConfig.h"
#include "S2_RVG.h"

#include <SM_Calc.h>
#include <SM_Test.h>

namespace s2
{

CircleShape::CircleShape() 
	: m_radius(0) 
{
}

CircleShape::CircleShape(const CircleShape& circle)
	: Shape(circle)
	, m_center(circle.m_center)
	, m_radius(circle.m_radius)
{
	UpdateBounding();
}

CircleShape::CircleShape(const sm::vec2& center, float radius)
	: m_center(center)
	, m_radius(radius)
{
	UpdateBounding();
}

bool CircleShape::IsContain(const sm::vec2& pos) const
{
	return sm::dis_pos_to_pos(m_center, pos) < SHAPE_NODE_RADIUS;
}

CircleShape* CircleShape::Clone() const
{
	return new CircleShape(*this);
}

bool CircleShape::IsIntersect(const sm::rect& rect) const
{
	sm::vec2 r_center = rect.Center();
	float center_dis = sm::dis_pos_to_pos(r_center, m_center);
	float r_radius = sm::dis_pos_to_pos(r_center, sm::vec2(rect.xmax, rect.ymax));
	if (center_dis > m_radius + r_radius) {
		return false;
	}

	if (sm::is_point_in_circle(sm::vec2(rect.xmin, rect.ymin), m_center, m_radius)) return true;
	if (sm::is_point_in_circle(sm::vec2(rect.xmax, rect.ymin), m_center, m_radius)) return true;
	if (sm::is_point_in_circle(sm::vec2(rect.xmax, rect.ymax), m_center, m_radius)) return true;
	if (sm::is_point_in_circle(sm::vec2(rect.xmin, rect.ymax), m_center, m_radius)) return true;
	if (sm::is_point_in_rect(m_center, rect)) return true;

	if (m_center.x > rect.xmin && m_center.x < rect.xmax) {
		if (center_dis < rect.ymax - r_center.y + m_radius) {
			return true;
		}
	}
	if (m_center.y > rect.ymin && m_center.y < rect.ymax) {
		if (center_dis < rect.xmax - r_center.x + m_radius) {
			return true;
		}
	}

	return false;
}

void CircleShape::Draw(const S2_MAT& mt, const RenderColor* color) const
{
	sm::vec2 c = mt * m_center;
	float r = sm::mat_trans_len(m_radius, mt);
	if (color) {
		RVG::SetColor(color->GetMul());
	} else {
		RVG::SetColor(0xffffffff);
	}
	RVG::Circle(c, r, false, 32);
}

const sm::vec2& CircleShape::GetCenter() const 
{ 
	return m_center; 
}

void CircleShape::SetCenter(const sm::vec2& center) 
{ 
	m_center = center;
	UpdateBounding();
}

float CircleShape::GetRadius() const 
{ 
	return m_radius; 
}

void CircleShape::SetRadius(float radius) 
{ 
	m_radius = radius;
	UpdateBounding();
}

void CircleShape::UpdateBounding()
{
	m_bounding = sm::rect(m_center, m_radius * 2, m_radius * 2);	
}

}