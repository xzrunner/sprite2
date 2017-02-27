#include "PointShape.h"
#include "ShapeConfig.h"
#include "S2_RVG.h"
#include "RenderParams.h"

#include <SM_Test.h>
#include <SM_Calc.h>

namespace s2
{

PointShape::PointShape() 
{
}

PointShape::PointShape(const PointShape& point)
	: Shape(point)
	, m_pos(point.m_pos)
{
	UpdateBounding();
}

PointShape::PointShape(const sm::vec2& pos) 
	: m_pos(pos) 
{
	UpdateBounding();
}

PointShape* PointShape::Clone() const
{
	return new PointShape(*this);
}

bool PointShape::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_rect(pos, m_bounding);	
}

bool PointShape::IsIntersect(const sm::rect& rect) const
{
	return sm::is_rect_intersect_rect(rect, m_bounding);
}

void PointShape::Draw(const RenderParams& rp) const
{
	sm::vec2 center = rp.mt * m_pos;
	float r = sm::mat_trans_len(SHAPE_NODE_RADIUS, rp.mt);
	RVG::SetColor(rp.color.GetMul());
	RVG::Circle(center, r, true);
}

const sm::vec2& PointShape::GetPos() const 
{ 
	return m_pos; 
}

void PointShape::SetPos(const sm::vec2& pos) 
{
	m_pos = pos;
	UpdateBounding();
}

void PointShape::UpdateBounding()
{
	m_bounding.xmin = m_bounding.ymin = -SHAPE_NODE_RADIUS;
	m_bounding.xmax = m_bounding.ymax =  SHAPE_NODE_RADIUS;
}

}