#include "sprite2/Polygon.h"
#include "sprite2/RenderParams.h"

#include <SM_Process.h>
#include <painting2/PrimitiveDraw.h>

namespace s2
{

void Polygon::Clear()
{
	m_tris.clear();
	m_outline.clear();
	m_segments.clear();
	m_holes.clear();
}

void Polygon::DebugDraw(const S2_MAT& mt) const
{
	CU_VEC<sm::vec2> buf;
	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		buf.push_back(m_tris[i]);
		if (buf.size() == 3)
		{
			CU_VEC<sm::vec2> vertices;
			sm::trans_vertices(mt, buf, vertices);
			pt2::PrimitiveDraw::SetColor(pt2::Color(0, 255, 0));
			pt2::PrimitiveDraw::Polyline(nullptr, vertices, true);
			buf.clear();
		}
	}
}

}