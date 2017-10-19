#include "ShapeVertices.h"
#include "S2_RVG.h"
#include "RenderParams.h"

namespace s2
{

void ShapeVertices::Draw(const CU_VEC<sm::vec2>& vertices, bool closed,
						 const RenderParams& rp)
{
	if (vertices.empty()) {
		return;
	}

	CU_VEC<sm::vec2> trans(vertices);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		trans[i] = rp.mt * vertices[i];
	}
	RVG::SetColor(rp.color.GetMul());
	RVG::Polyline(trans, closed);
}

}