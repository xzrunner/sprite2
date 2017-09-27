#include "ColorPolygon.h"
#include "S2_RVG.h"
#include "RenderColor.h"
#include "RenderParams.h"

#include <SM_Process.h>
#include <SM_Triangulation.h>

namespace s2
{

ColorPolygon::ColorPolygon(const Color& color)
	: m_color(color)
{
}

void ColorPolygon::Draw(const RenderParams& rp) const
{
	std::vector<sm::vec2> tris;
	sm::trans_vertices(rp.mt, m_tris, tris);
	RVG::SetColor(m_color * rp.color.GetMul());
	RVG::Triangles(tris);
}

void ColorPolygon::Build()
{
	m_tris.clear();

	std::vector<sm::vec2> outline;
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