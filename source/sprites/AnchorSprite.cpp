#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "UpdateParams.h"
#include "SpriteVisitor.h"

namespace s2
{

void AnchorSprite::OnMessage(const UpdateParams& up, Message msg)
{
	auto actor = up.GetActor();
	auto anchor = QueryAnchor(actor);
	if (!anchor) {
		return;
	}

	UpdateParams up_child(up);

	up_child.Push(this);
	auto anchor_spr = anchor->GetSprRaw();
	up_child.SetActor(anchor_spr->QueryActor(actor));
	const_cast<Sprite*>(anchor_spr)->OnMessage(up_child, msg);
}

bool AnchorSprite::Update(const UpdateParams& up)
{
	auto actor = up.GetActor();
	auto anchor = QueryAnchor(actor);
	if (!anchor) {
		return false;
	}

	auto spr_real = anchor->GetSprRaw();

	// update inherit
	if (!up.IsForce() && !spr_real->IsInheritUpdate()) {
		return false;
	}

	auto actor_real = spr_real->QueryActor(actor);
	
	// visible
	bool visible = actor_real ? actor_real->IsVisible() : spr_real->IsVisible();
	if (!visible) {
		return false;
	}

	UpdateParams up_child(up);

	up_child.Push(this);
	up_child.SetActor(actor_real);
	bool ret = const_cast<Sprite*>(spr_real)->Update(up_child);

	return ret;
}

SprPtr AnchorSprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	auto anchor = QueryAnchor(actor.get());
	if (anchor) {
		auto anchor_spr = anchor->GetSprRaw();
		return anchor_spr->FetchChildByName(name, anchor_spr->QueryActorRef(actor.get()));
	} else {
		return nullptr;
	}
}

SprPtr AnchorSprite::FetchChildByIdx(int idx, const ActorPtr& actor) const
{
	auto anchor = QueryAnchor(actor.get());
	if (anchor) {
		auto anchor_spr = anchor->GetSprRaw();
		return anchor_spr->FetchChildByIdx(idx, anchor_spr->QueryActorRef(actor.get()));
	} else {
		return nullptr;
	}
}

VisitResult AnchorSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	auto& actor = params.actor;
	auto anchor = actor ? static_cast<const AnchorActor*>(actor)->GetAnchorPtr() : nullptr;
	if (anchor) {
		SprVisitorParams cp = params;
		cp.actor = anchor.get();
		SpriteVisitor::VisitChild(visitor, cp, anchor->GetSpr(), ret);
	}
	return ret;
}

VisitResult AnchorSprite::TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const
{
	auto& actor = params.actor;
	auto anchor = actor ? S2_VI_PTR_DOWN_CAST<const AnchorActor>(actor)->GetAnchorPtr() : nullptr;
	if (anchor) {
		SprVisitorParams2 cp = params;
		cp.actor = anchor;
		return anchor->GetSprRaw()->TraverseChildren2(visitor, cp);
	} else {
		return VISIT_OVER;
	}
}

const Actor* AnchorSprite::QueryAnchor(const Actor* actor) const
{
	return actor ? S2_VI_DOWN_CAST<const AnchorActor*>(actor)->GetAnchor() : nullptr;
}

}