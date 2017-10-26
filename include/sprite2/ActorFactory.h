#ifndef _SPRITE2_ACTOR_FACTORY_H_
#define _SPRITE2_ACTOR_FACTORY_H_

#include "sprite2/typedef.h"

#include <memory>

namespace s2
{

class Actor;
class Sprite;

class ActorFactory
{
public:
	static ActorPtr Create(const ActorConstPtr& parent, 
		const SprConstPtr& child);

}; // ActorFactory

}

#endif // _SPRITE2_ACTOR_FACTORY_H_