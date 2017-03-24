#include "AnchorSymbol.h"
#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "DrawNode.h"

#include <assert.h>

namespace s2
{

int AnchorSymbol::Type() const 
{ 
	return SYM_ANCHOR; 
}

void AnchorSymbol::Traverse(const SymbolVisitor& visitor)
{
	// todo: path
}

void AnchorSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	assert(spr);
	const Actor* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (anchor) {
		RenderParams rp_child = rp;
		rp_child.actor = anchor->GetSpr()->QueryActor(rp.actor);
		DrawNode::Draw(anchor->GetSpr(), rp_child, false);
	}
}

bool AnchorSymbol::Update(const RenderParams& rp, float time)
{
	// todo
	return false;
}

sm::rect AnchorSymbol::GetBounding(const Sprite* spr) const
{
	// todo: path
	return sm::rect(100, 100);
}

}