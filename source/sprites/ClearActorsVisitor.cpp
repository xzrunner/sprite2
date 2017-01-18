#include "ClearActorsVisitor.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"

namespace s2
{

VisitResult ClearActorsVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{
	const_cast<Sprite*>(spr)->ClearActors();

	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_INVALID || type == SYM_UNKNOWN) {
		return VISIT_IGNORE;
	}
	if (type == SYM_COMPLEX || type == SYM_ANIMATION) {
		return VISIT_INTO;
	}

	return VISIT_CONTINUE;
}

}