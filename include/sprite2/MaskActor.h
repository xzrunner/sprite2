#ifndef _SPRITE2_MASK_ACTOR_H_
#define _SPRITE2_MASK_ACTOR_H_

#include "sprite2/Actor.h"

namespace s2
{

class MaskActor : public Actor
{
public:
	MaskActor(const SprConstPtr& spr, const ActorConstPtr& parent);

}; // MaskActor

}

#endif // _SPRITE2_MASK_ACTOR_H_