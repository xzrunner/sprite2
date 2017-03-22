#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "RenderParams.h"

namespace s2
{

void AnchorSprite::OnMessage(Message msg, const SprTreePath& path)
{
	SprTreePath cpath = path;
	cpath.Push(*this);
	const Sprite* anchor = QueryAnchor(cpath);
	if (anchor) {
		cpath.Clear();
		const_cast<Sprite*>(anchor)->OnMessage(msg, cpath);
	}
}

bool AnchorSprite::Update(const RenderParams& rp)
{
	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	rp_child.path.Push(*this);

	const Sprite* anchor = QueryAnchor(rp_child.path);
	if (anchor) {
		rp_child.path.Clear();
		return const_cast<Sprite*>(anchor)->Update(rp_child);
	} else {
		return false;
	}
}

bool AnchorSprite::SetFrame(int frame, const SprTreePath& path, bool force)
{
	bool dirty = false;
	SprTreePath cpath = path;
	cpath.Push(*this);
	const Sprite* anchor = QueryAnchor(cpath);
	if (anchor) {
		cpath.Clear();
		const_cast<Sprite*>(anchor)->SetFrame(frame, cpath);
		dirty = true;
	}
	return dirty;
}

Sprite* AnchorSprite::FetchChild(const std::string& name, const SprTreePath& path) const
{
	SprTreePath cpath = path;
	cpath.Push(*this);
	const Sprite* anchor = QueryAnchor(cpath);
	if (anchor) {
		return anchor->FetchChild(name, cpath);
	} else {
		return NULL;
	}
}

Sprite* AnchorSprite::FetchChild(int idx, const SprTreePath& path) const
{
	SprTreePath cpath = path;
	cpath.Push(*this);
	const Sprite* anchor = QueryAnchor(cpath);
	if (anchor) {
		return anchor->FetchChild(idx, cpath);
	} else {
		return NULL;
	}
}

VisitResult AnchorSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Sprite* anchor = QueryAnchor(params.path);
	if (anchor) {
		SprVisitorParams p_child = params;
		p_child.path.Clear();
		return anchor->TraverseChildren(visitor, p_child);
	} else {
		return VISIT_OVER;
	}
}

void AnchorSprite::AddAnchor(const Sprite* anchor, const SprTreePath& path)
{
	AnchorActor* actor = VI_DOWNCASTING<AnchorActor*>(ActorFactory::Instance()->Create(path, this));
	actor->SetAnchor(anchor);
}

const Sprite* AnchorSprite::QueryAnchor(const SprTreePath& path) const
{
	const Actor* actor = QueryActor(path);
	if (!actor) {
		return NULL;
	}
	const AnchorActor* anchor_actor = static_cast<const AnchorActor*>(actor);
	return anchor_actor->GetAnchor();
}

}