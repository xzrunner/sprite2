#ifndef _SPRITE2_ANIM_ACTOR_H_
#define _SPRITE2_ANIM_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class AnimCurr;

class AnimActor : public Actor
{
public:
	AnimActor(const Sprite* spr, const Actor* parent);
	virtual ~AnimActor();

	const AnimCurr* GetCurr() const { return m_curr; }

private:
	AnimCurr* m_curr;

}; // AnimActor

}

#endif // _SPRITE2_ANIM_ACTOR_H_