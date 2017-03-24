#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "RenderParams.h"

namespace s2
{

void AnchorSprite::OnMessage(Message msg, const Actor* actor)
{
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		const Sprite* anchor_spr = anchor->GetSpr();
		const_cast<Sprite*>(anchor_spr)->OnMessage(msg, anchor_spr->QueryActor(actor));
	}
}

bool AnchorSprite::Update(const RenderParams& rp)
{
	const Actor* actor = rp.actor;
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		RenderParams rp_child = rp;
		rp_child.mt = GetLocalMat() * rp.mt;
		rp_child.shader = GetShader() * rp.shader;
		rp_child.actor = anchor->GetSpr()->QueryActor(actor);
		return const_cast<Sprite*>(anchor->GetSpr())->Update(rp_child);
	} else {
		return false;
	}
}

bool AnchorSprite::SetFrame(int frame, const Actor* actor, bool force)
{
	bool dirty = false;
	const Actor* anchor = QueryAnchor(actor);
	if (anchor) {
		const Sprite* anchor_spr = anchor->GetSpr();
		const_cast<Sprite*>(anchor_spr)->SetFrame(frame, anchor_spr->QueryActor(actor));
		dirty = true;
	}
	return dirty;
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