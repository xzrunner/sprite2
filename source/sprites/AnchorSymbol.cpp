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
// <<<<<<< HEAD
// 	const Sprite* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
// 	if (anchor) {
// 		rp_child.actor = spr->QueryActor(rp.actor);
// 		DrawNode::Draw(anchor, rp_child, false);
// =======
	const Actor* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (anchor) 
	{
		RenderParams rp_child = rp;
 		const Actor* anchor_actor = anchor->GetSpr()->QueryActor(rp.actor);
		rp_child.actor = anchor_actor;
		DrawNode::Draw(anchor->GetSpr(), rp_child, false);
//>>>>>>> B-anchor-sr
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