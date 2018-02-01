#include "sprite2/RectShape.h"
#include "sprite2/RVG.h"
#include "sprite2/RenderParams.h"

#include <SM_Test.h>

namespace s2
{

RectShape::RectShape(const RectShape& rect)
	: Shape(rect)
{
}

RectShape::RectShape(const sm::rect& r) 
{
	m_bounding = r;
}

RectShape* RectShape::Clone() const
{
	return new RectShape(*this);
}

bool RectShape::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_rect(pos, m_bounding);
}

bool RectShape::IsIntersect(const sm::rect& rect) const
{
	return sm::is_rect_intersect_rect(rect, m_bounding);
}

void RectShape::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	RVG::SetColor(rp.col_common.mul);
	sm::vec2 min(m_bounding.xmin, m_bounding.ymin),
		     max(m_bounding.xmax, m_bounding.ymax);
	RVG::Rect(dlist, min, max, false);
}

}