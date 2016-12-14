#ifndef _SPRITE2_DUMMY_ACTOR_H_
#define _SPRITE2_DUMMY_ACTOR_H_

#include "S2_Actor.h"

#include <string>

namespace s2
{

class DummyActor : public Actor
{
public:
	DummyActor(const Sprite* spr, const SprTreePath& path)
		: Actor(spr, path) {}

}; // DummyActor

}

#endif // _SPRITE2_DUMMY_ACTOR_H_