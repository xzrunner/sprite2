#include "FixActorPathVisitor.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"

#include <assert.h>

namespace s2
{

FixActorPathVisitor::FixActorPathVisitor(const SprTreePath& parent_path)
	: m_parent_path(parent_path)
{
}

VisitResult FixActorPathVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	const_cast<Sprite*>(spr)->FixActorPath(m_parent_path);

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	return VISIT_CONTINUE;
}

void FixActorPathVisitor::VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params)
{
	m_parent_path.Push(spr->GetID());
}

void FixActorPathVisitor::VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params)
{
	assert(!m_parent_path.Empty() && m_parent_path.Top() == spr->GetID());
	m_parent_path.Pop();
}

}