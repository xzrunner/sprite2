#ifndef _SPRITE2_ANCHOR_ACTOR_H_
#define _SPRITE2_ANCHOR_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class AnchorActor : public Actor
{
public:
	AnchorActor(const SprConstPtr& spr, const ActorConstPtr& parent)
		: Actor(spr, parent) {}

	void SetAnchor(const ActorPtr& anchor);
	const ActorPtr& GetAnchor() const { return m_anchor; }

private:
	void Clear();

private:
	ActorPtr m_anchor;

}; // AnchorActor

}

#endif // _SPRITE2_ANCHOR_ACTOR_H_