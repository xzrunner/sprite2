#ifndef _SPRITE2_ANIM_ACTOR_H_
#define _SPRITE2_ANIM_ACTOR_H_

#include "S2_Actor.h"
#include "sprite2/AnimState.h"
#include "sprite2/AnimCurr.h"

namespace s2
{

class AnimActor : public Actor
{
public:
	AnimActor(const SprConstPtr& spr, const ActorConstPtr& parent);

	AnimState& GetState() { return m_state; }
	const AnimState& GetState() const { return m_state; }

private:
	AnimState m_state;

}; // AnimActor

}

#endif // _SPRITE2_ANIM_ACTOR_H_