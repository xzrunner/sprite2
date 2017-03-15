#include "AnchorSymbol.h"
#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "DrawNode.h"

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
	RenderParams rp_child;
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

	assert(spr);
	const Sprite* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp_child.path);
	if (anchor) {
		DrawNode::Draw(anchor, rp_child, false);
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