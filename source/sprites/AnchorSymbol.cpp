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

void AnchorSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	assert(spr);
	const Actor* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (!anchor) {
		return;
	}

	RenderParams rp_child(rp);
	rp_child.actor = anchor->GetSpr()->QueryActor(rp.actor);
	rp_child.mt = spr->GetLocalMat() * rp_child.mt;
	DrawNode::Draw(anchor->GetSpr(), rp_child, false);
}

sm::rect AnchorSymbol::GetBounding(const Sprite* spr, const Actor* actor) const
{
	return sm::rect(100, 100);
}

}