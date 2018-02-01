#include "sprite2/ColorPolygon.h"
#include "sprite2/RVG.h"
#include "sprite2/RenderParams.h"

#include <SM_Process.h>
#include <SM_Triangulation.h>

namespace s2
{

ColorPolygon::ColorPolygon(const pt2::Color& color)
	: m_color(color)
{
}

void ColorPolygon::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	CU_VEC<sm::vec2> tris;
	sm::trans_vertices(rp.mt, m_tris, tris);
	RVG::SetColor(m_color * rp.col_common.mul);
	RVG::Triangles(dlist, tris);
}

void ColorPolygon::Build()
{
	m_tris.clear();

	CU_VEC<sm::vec2> outline;
	sm::rm_duplicate_nodes(m_outline, outline);

	if (!m_segments.empty()) {
		sm::triangulate_lines(m_outline, m_segments, m_tris);
	} else if (!m_holes.empty()) {
		sm::triangulate_holes(m_outline, m_holes, m_tris);
	} else {
		sm::triangulate_normal(m_outline, m_tris);
	}
}

}