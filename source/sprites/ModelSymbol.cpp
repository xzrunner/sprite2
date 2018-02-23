#include "sprite2/ModelSymbol.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/SymType.h"

namespace s2
{

ModelSymbol::ModelSymbol()
{
}

int ModelSymbol::Type() const
{
	return SYM_MODEL;
}

pt2::RenderReturn ModelSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	return pt2::RENDER_OK;
}

sm::rect ModelSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	// empty
	return sm::rect();
}

}

#endif // S2_DISABLE_MODEL