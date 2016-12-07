#include "PointQueryVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "BoundingBox.h"

#include "S2_RVG.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
	, m_spr(NULL)
{
}

PointQueryVisitor::~PointQueryVisitor()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

VisitResult PointQueryVisitor::Visit(const Sprite* spr, const sm::mat4* mat)
{
	if (m_spr) {
		return VISIT_STOP;
	}

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	assert(mat);
	sm::mat4 m = spr->GetLocalMat() * (*mat);
	sm::rect sz = spr->GetSymbol()->GetBounding(spr);
	std::vector<sm::vec2> vertices(4);
	vertices[0] = sm::vec2(sz.xmin, sz.ymin);
	vertices[1] = sm::vec2(sz.xmin, sz.ymax);
	vertices[2] = sm::vec2(sz.xmax, sz.ymax);
	vertices[3] = sm::vec2(sz.xmax, sz.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = m * vertices[i];
	}
	RVG::Polyline(vertices, true);
	if (sm::is_point_in_convex(m_pos, vertices)) {	
		spr->AddReference();
		m_spr = spr;
		m_mat = m;
		return VISIT_STOP;
	} else {
		return VISIT_CONTINUE;
	}
}

}