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

RenderReturn AnchorSymbol::DrawTree(const RenderParams& rp, const Sprite* spr) const
{
	assert(spr);
	auto anchor = S2_VI_DOWN_CAST<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (!anchor) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	rp_child->actor = GetRealActor(spr, rp.actor.get());
	if (rp_child->actor) {
		S2_MAT m;
		sm::Matrix2D::Mul(spr->GetLocalMat(), rp_child->mt, m);
		rp_child->mt = m;
//		rp_child->SetDisableCulling(true);
		ret = DrawNode::Draw(rp_child->actor->GetSpr().get(), *rp_child);
	} else {
		ret = RENDER_NO_DATA;
	}
	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn AnchorSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	return RENDER_SKIP;
}

sm::rect AnchorSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	auto real_actor = GetRealActor(spr, actor);
	if (real_actor) {
		return real_actor->GetSpr()->GetBounding(real_actor.get())->GetSize();
	} else {
		return sm::rect(); // empty
	}
}

ActorConstPtr AnchorSymbol::GetRealActor(const Sprite* spr, const Actor* actor)
{
	if (spr && actor) 
	{
		auto parent = actor->GetParent();
		if (!parent) {
			return nullptr;
		}

		auto anchor_actor = spr->QueryActor(parent.get());
		if (anchor_actor) {
			return std::static_pointer_cast<const AnchorActor>(anchor_actor)->GetAnchor();
		}
	}	
	return nullptr;
}

}