#include "PointQueryVisitor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "BoundingBox.h"

namespace s2
{

PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
	, m_result(NULL)
{
}

PointQueryVisitor::~PointQueryVisitor()
{
	if (m_result) {
		m_result->RemoveReference();
	}
}

VisitResult PointQueryVisitor::Visit(const Sprite* spr)
{
	if (m_result) {
		return VISIT_STOP;
	}

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}
	if (spr->GetBounding()->IsContain(m_pos)) {
		spr->AddReference();
		m_result = spr;
		return VISIT_STOP;
	} else {
		return VISIT_CONTINUE;
	}
}

}