#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "RenderParams.h"

namespace s2
{

void AnchorSprite::OnMessage(Message msg, const Actor* actor)
{
	const Sprite* anchor = QueryAnchor(actor)->GetSpr();
	if (anchor) {
		const_cast<Sprite*>(anchor)->OnMessage(msg, anchor->QueryActor(actor));
	}
}

bool AnchorSprite::Update(const RenderParams& rp)
{
	const Actor* actor = rp.actor;
	const Sprite* anchor = QueryAnchor(actor)->GetSpr();
	if (anchor) {
		RenderParams rp_child = rp;
		rp_child.mt = GetLocalMat() * rp.mt;
		rp_child.shader = GetShader() * rp.shader;
		rp_child.actor = anchor->QueryActor(actor);
		return const_cast<Sprite*>(anchor)->Update(rp_child);
	} else {
		return false;
	}
}

bool AnchorSprite::SetFrame(int frame, const Actor* actor, bool force)
{
	bool dirty = false;
	const Sprite* anchor = QueryAnchor(actor)->GetSpr();
	if (anchor) {
		const_cast<Sprite*>(anchor)->SetFrame(frame, anchor->QueryActor(actor));
		dirty = true;
	}
	return dirty;
}

Sprite* AnchorSprite::FetchChild(const std::string& name, const Actor* actor) const
{
	const Sprite* anchor = QueryAnchor(actor)->GetSpr();
	if (anchor) {
		return anchor->FetchChild(name, anchor->QueryActor(actor));
	} else {
		return NULL;
	}
}

Sprite* AnchorSprite::FetchChild(int idx, const Actor* actor) const
{
	const Sprite* anchor = QueryAnchor(actor)->GetSpr();
	if (anchor) {
		return anchor->FetchChild(idx, anchor->QueryActor(actor));
	} else {
		return NULL;
	}
}

VisitResult AnchorSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Actor* actor = params.actor;
	const Sprite* anchor = QueryAnchor(actor)->GetSpr();
	if (anchor) {
		SprVisitorParams cp = params;
		return anchor->TraverseChildren(visitor, cp);
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