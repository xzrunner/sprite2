#include "FixActorPathVisitor.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"

#include <assert.h>

namespace s2
{

FixActorPathVisitor::FixActorPathVisitor(const SprTreePath& path)
	: m_path(path)
{
}

VisitResult FixActorPathVisitor::Visit(const Sprite* spr, const sm::mat4* mat)
{
	const_cast<Sprite*>(spr)->FixActorPath(m_path);

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	return VISIT_CONTINUE;
}

void FixActorPathVisitor::VisitChildrenBegin(const Sprite* spr)
{
	m_path.Push(spr->GetID());
}

void FixActorPathVisitor::VisitChildrenEnd(const Sprite* spr)
{
	assert(!m_path.Empty() && m_path.Top() == spr->GetID());
	m_path.Pop();
}

}