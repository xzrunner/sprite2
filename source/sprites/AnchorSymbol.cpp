#include "sprite2/AnchorSymbol.h"
#include "sprite2/AnchorSprite.h"
#include "sprite2/AnchorActor.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Sprite.h"

#include <memmgr/Allocator.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

#include <assert.h>

namespace s2
{

int AnchorSymbol::Type() const 
{ 
	return SYM_ANCHOR; 
}

RenderReturn AnchorSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	assert(spr);
	auto anchor = S2_VI_DOWN_CAST<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (!anchor) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->actor = GetRealActor(spr, rp.actor);
	if (rp_child->actor) {
		S2_MAT m;
		sm::Matrix2D::Mul(spr->GetLocalMat(), rp_child->mt, m);
		rp_child->mt = m;
//		rp_child->SetDisableCulling(true);
		ret = DrawNode::Draw(dlist, rp_child->actor->GetSprRaw(), *rp_child);
	} else {
		ret = RENDER_NO_DATA;
	}

	return ret;
}

sm::rect AnchorSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	auto real_actor = GetRealActor(spr, actor);
	if (real_actor) {
		return real_actor->GetSprRaw()->GetBounding(real_actor).GetSize();
	} else {
		return sm::rect(); // empty
	}
}

const Actor* AnchorSymbol::GetRealActor(const Sprite* spr, const Actor* actor)
{
	if (spr && actor) 
	{
		auto parent = actor->GetParent();
		if (!parent) {
			return nullptr;
		}

		auto anchor_actor = spr->QueryActor(parent.get());
		if (anchor_actor) {
			return S2_VI_DOWN_CAST<const AnchorActor*>(anchor_actor)->GetAnchor();
		}
	}	
	return nullptr;
}

}