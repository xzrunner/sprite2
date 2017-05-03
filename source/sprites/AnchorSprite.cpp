#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "UpdateParams.h"

namespace s2
{

void AnchorSprite::OnMessage(const UpdateParams& up, Message msg)
{
	const Actor* actor = up.GetActor();
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		UpdateParams up_child(up);
		up_child.Push(this);
		const Sprite* anchor_spr = anchor->GetSpr();
		up_child.SetActor(anchor_spr->QueryActor(actor));
		const_cast<Sprite*>(anchor_spr)->OnMessage(up_child, msg);
	}
}

bool AnchorSprite::Update(const UpdateParams& up)
{
	const Actor* actor = up.GetActor();
	const Actor* anchor = QueryAnchor(actor);
	if (!actor) {
		return false;
	}
	if (!up.IsForce() && !anchor->GetSpr()->IsInheritUpdate()) {
		return false;
	}
	UpdateParams up_child(up);
	up_child.Push(this);
	up_child.SetActor(anchor->GetSpr()->QueryActor(actor));
	return const_cast<Sprite*>(anchor->GetSpr())->Update(up_child);
}

Sprite* AnchorSprite::FetchChild(const std::string& name, const Actor* actor) const
{
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		const Sprite* anchor_spr = anchor->GetSpr();
		return anchor_spr->FetchChild(name, anchor_spr->QueryActor(actor));
	} else {
		return NULL;
	}
}

Sprite* AnchorSprite::FetchChild(int idx, const Actor* actor) const
{
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		const Sprite* anchor_spr = anchor->GetSpr();
		return anchor_spr->FetchChild(idx, anchor_spr->QueryActor(actor));
	} else {
		return NULL;
	}
}

VisitResult AnchorSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Actor* actor = params.actor;
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		SprVisitorParams cp = params;
		cp.actor = anchor;
		return anchor->GetSpr()->TraverseChildren(visitor, cp);
	} else {
		return VISIT_OVER;
	}
}

void AnchorSprite::AddAnchor(const Actor* child, const Actor* parent)
{
	AnchorActor* actor = VI_DOWNCASTING<AnchorActor*>(ActorFactory::Instance()->Create(parent, this));
	actor->SetAnchor(child);
}

const Actor* AnchorSprite::QueryAnchor(const Actor* actor) const
{
	if (actor) {
		const AnchorActor* anchor_actor = static_cast<const AnchorActor*>(actor);
		return anchor_actor->GetAnchor();
	} else {
		return NULL;
	}
}

}