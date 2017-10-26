#ifndef _SPRITE2_ANCHOR_ACTOR_H_
#define _SPRITE2_ANCHOR_ACTOR_H_

#include "sprite2/Actor.h"

namespace s2
{

class AnchorActor : public Actor
{
public:
	AnchorActor(const SprConstPtr& spr, const ActorConstPtr& parent)
		: Actor(spr, parent) {}

	void SetAnchor(const ActorPtr& anchor);
	const Actor* GetAnchor() const { return m_anchor.actor.get(); }
	const ActorPtr& GetAnchorPtr() const { return m_anchor.actor; }

private:
	void Clear();

private:
	struct Anchor
	{
		ActorPtr actor;
		SprPtr   spr;
	};

	Anchor m_anchor;

}; // AnchorActor

}

#endif // _SPRITE2_ANCHOR_ACTOR_H_