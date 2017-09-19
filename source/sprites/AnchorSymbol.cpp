#include "AnchorSymbol.h"
#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SymType.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "sprite2/Flatten.h"
#include "FlattenParams.h"
#include "DrawNodeDeferred.h"

#include <assert.h>

namespace s2
{

int AnchorSymbol::Type() const 
{ 
	return SYM_ANCHOR; 
}

RenderReturn AnchorSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	assert(spr);
	const Actor* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (!anchor) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	rp_child->actor = GetRealActor(spr, rp.actor);
	if (rp_child->actor) {
		S2_MAT m;
		sm::Matrix2D::Mul(spr->GetLocalMat(), rp_child->mt, m);
		rp_child->mt = m;
//		rp_child->SetDisableCulling(true);
		ret = DrawNode::Draw(rp_child->actor->GetSpr(), *rp_child);
	} else {
		ret = RENDER_NO_DATA;
	}
	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn AnchorSymbol::DrawDeferred(cooking::DisplayList* dlist, const 
										RenderParams& rp, 
										const Sprite* spr) const
{
	assert(spr);
	const Actor* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(rp.actor);
	if (!anchor) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	rp_child->actor = GetRealActor(spr, rp.actor);
	if (rp_child->actor) {
		S2_MAT m;
		sm::Matrix2D::Mul(spr->GetLocalMat(), rp_child->mt, m);
		rp_child->mt = m;
//		rp_child->SetDisableCulling(true);
		ret = DrawNodeDeferred::Draw(dlist, rp_child->actor->GetSpr(), *rp_child);
	} else {
		ret = RENDER_NO_DATA;
	}
	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

void AnchorSymbol::Flattening(const FlattenParams& fp, Flatten& ft) const
{
	const Sprite* spr = fp.GetSpr();
	const Actor* actor = fp.GetActor();
	assert(spr);
	const Actor* anchor = VI_DOWNCASTING<const AnchorSprite*>(spr)->QueryAnchor(actor);
	if (!anchor) {
		return;
	}

	const Actor* real_actor = GetRealActor(spr, actor);
	if (real_actor) 
	{
		const Sprite* real_spr = real_actor->GetSpr();
		FlattenParams c_fp = fp;
		c_fp.SetSpr(real_spr);
		c_fp.SetActor(real_actor);
		real_spr->GetSymbol()->Flattening(c_fp, ft);
	}
}

sm::rect AnchorSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	const Actor* real_actor = GetRealActor(spr, actor);
	if (real_actor) {
		return real_actor->GetSpr()->GetBounding(real_actor)->GetSize();
	} else {
		return sm::rect(); // empty
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