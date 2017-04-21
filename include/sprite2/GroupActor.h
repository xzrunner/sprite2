#ifndef _SPRITE2_GROUP_ACTOR_H_
#define _SPRITE2_GROUP_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class GroupActor : public Actor
{
public:
	GroupActor(const Sprite* spr, const Actor* parent);

}; // GroupActor

}

#endif // _SPRITE2_GROUP_ACTOR_H_