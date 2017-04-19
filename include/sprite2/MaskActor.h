#ifndef _SPRITE2_MASK_ACTOR_H_
#define _SPRITE2_MASK_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class MaskActor : public Actor
{
public:
	MaskActor(const Sprite* spr, const Actor* parent);

}; // MaskActor

}

#endif // _SPRITE2_MASK_ACTOR_H_