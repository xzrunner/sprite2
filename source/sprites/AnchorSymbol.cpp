#include "AnchorSymbol.h"
#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"

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
	rp_child.actor = GetRealActor(spr, rp.actor);
	if (rp_child.actor) {
		rp_child.mt = spr->GetLocalMat() * rp_child.mt;
		DrawNode::Draw(rp_child.actor->GetSpr(), rp_child, false);
	}
}

sm::rect AnchorSymbol::GetBounding(const Sprite* spr, const Actor* actor) const
{
	const Actor* real_actor = GetRealActor(spr, actor);
	if (real_actor) {
		return real_actor->GetSpr()->GetBounding(real_actor)->GetSize();
	} else {
		return sm::rect(100, 100);
	}
}

const Actor* AnchorSymbol::GetRealActor(const Sprite* spr, const Actor* actor)
{
	if (spr && actor) {
		const AnchorActor* anchor_actor = VI_DOWNCASTING<const AnchorActor*>(spr->QueryActor(actor->GetParent()));
		if (anchor_actor) {
			return anchor_actor->GetAnchor();
		}
	}	
	return NULL;
}

}