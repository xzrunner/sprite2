#ifndef _SPRITE2_SCALE9_ACTOR_H_
#define _SPRITE2_SCALE9_ACTOR_H_

#include "S2_Actor.h"
#include "Scale9.h"

namespace s2
{

class Scale9Actor : public Actor
{
public:
	Scale9Actor(const SprConstPtr& spr, const ActorConstPtr& parent);

	const Scale9& GetScale9() const { return m_s9; }

	void Resize(float width, float height);

private:
	Scale9 m_s9;

}; // Scale9Actor

}

#endif // _SPRITE2_SCALE9_ACTOR_H_