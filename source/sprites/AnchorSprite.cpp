#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "SprVisitorParams.h"
#include "ActorFactory.h"
#include "RenderParams.h"

namespace s2
{

bool AnchorSprite::Update(const RenderParams& rp)
{
	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	rp_child.path.Push(GetID());

	const Sprite* anchor = QueryAnchor(rp_child.path);
	if (anchor) {
		return const_cast<Sprite*>(anchor)->Update(rp);
	} else {
		return false;
	}
}

bool AnchorSprite::SetFrame(int frame, const SprTreePath& parent_path, bool force)
{
	bool dirty = false;
	SprTreePath path = parent_path;
	path.Push(GetID());
	const Sprite* anchor = QueryAnchor(path);
	if (anchor) {
		const_cast<Sprite*>(anchor)->SetFrame(frame, path);
		dirty = true;
	}
	return dirty;
}

Sprite* AnchorSprite::FetchChild(const std::string& name, const SprTreePath& path) const
{
	const Sprite* anchor = QueryAnchor(path);
	if (anchor) {
		return anchor->FetchChild(name, path);
	} else {
		return NULL;
	}
}

Sprite* AnchorSprite::FetchChild(int idx, const SprTreePath& path) const
{
	const Sprite* anchor = QueryAnchor(path);
	if (anchor) {
		return anchor->FetchChild(idx, path);
	} else {
		return NULL;
	}
}

VisitResult AnchorSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Sprite* anchor = QueryAnchor(params.path);
	if (anchor) {
		SprVisitorParams p_child = params;
		p_child.path.Push(GetID());
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