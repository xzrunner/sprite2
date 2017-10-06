#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "UpdateParams.h"

namespace s2
{

void AnchorSprite::OnMessage(const UpdateParams& up, Message msg)
{
	auto& actor = up.GetActor();
	auto& anchor = QueryAnchor(actor);
	if (!anchor) {
		return;
	}

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;

	up_child->Push(shared_from_this());
	auto& anchor_spr = anchor->GetSpr();
	up_child->SetActor(anchor_spr->QueryActor(actor.get()));
	std::const_pointer_cast<Sprite>(anchor_spr)->OnMessage(*up_child, msg);

	UpdateParamsPool::Instance()->Push(up_child); 
}

bool AnchorSprite::Update(const UpdateParams& up)
{
	auto& actor = up.GetActor();
	auto& anchor = QueryAnchor(actor);
	if (!anchor) {
		return false;
	}

	auto& spr_real = anchor->GetSpr();

	// update inherit
	if (!up.IsForce() && !spr_real->IsInheritUpdate()) {
		return false;
	}

	auto& actor_real = spr_real->QueryActor(actor.get());
	
	// visible
	bool visible = actor_real ? actor_real->IsVisible() : spr_real->IsVisible();
	if (!visible) {
		return false;
	}

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;

	up_child->Push(shared_from_this());
	up_child->SetActor(actor_real);
	bool ret = std::const_pointer_cast<Sprite>(spr_real)->Update(*up_child);

	UpdateParamsPool::Instance()->Push(up_child); 

	return ret;
}

SprPtr AnchorSprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	auto& anchor = QueryAnchor(actor);
	if (anchor) {
		auto& anchor_spr = anchor->GetSpr();
		return anchor_spr->FetchChildByName(name, anchor_spr->QueryActor(actor.get()));
	} else {
		return nullptr;
	}
}

SprPtr AnchorSprite::FetchChildByIdx(int idx, const ActorPtr& actor) const
{
	auto& anchor = QueryAnchor(actor);
	if (anchor) {
		auto& anchor_spr = anchor->GetSpr();
		return anchor_spr->FetchChildByIdx(idx, anchor_spr->QueryActor(actor.get()));
	} else {
		return nullptr;
	}
}

VisitResult AnchorSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	auto& actor = params.actor;
	auto& anchor = QueryAnchor(actor);
	if (anchor) {
		SprVisitorParams cp = params;
		cp.actor = anchor;
		return anchor->GetSpr()->TraverseChildren(visitor, cp);
	} else {
		return VISIT_OVER;
	}
}

ActorConstPtr AnchorSprite::QueryAnchor(const ActorConstPtr& actor) const
{
	if (actor) {
		return std::static_pointer_cast<const AnchorActor>(actor)->GetAnchor();
	} else {
		return nullptr;
	}
}

}