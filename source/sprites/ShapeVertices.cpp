#include "sprite2/ShapeVertices.h"
#include "sprite2/RenderParams.h"

#include <painting2/PrimitiveDraw.h>

namespace s2
{

void ShapeVertices::Draw(cooking::DisplayList* dlist,
	                     const CU_VEC<sm::vec2>& vertices, 
	                     bool closed,
						 const RenderParams& rp)
{
	if (vertices.empty()) {
		return;
	}

	CU_VEC<sm::vec2> trans(vertices);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		trans[i] = rp.mt * vertices[i];
	}
	pt2::PrimitiveDraw::SetColor(rp.col_common.mul);
	pt2::PrimitiveDraw::Polyline(dlist, trans, closed);
}

}