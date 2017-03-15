#ifndef _SPRITE2_ANCHOR_ACTOR_H_
#define _SPRITE2_ANCHOR_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class AnchorActor : public Actor
{
public:
	AnchorActor(const Sprite* spr, const SprTreePath& path);
	virtual ~AnchorActor();

	void SetAnchor(const Sprite* anchor);
	const Sprite* GetAnchor() const;

private:
	const Sprite* m_anchor;

}; // AnchorActor

}

#endif // _SPRITE2_ANCHOR_ACTOR_H_