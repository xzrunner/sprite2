#ifndef _SPRITE2_ACTOR_FACTORY_H_
#define _SPRITE2_ACTOR_FACTORY_H_

#include <CU_Singleton.h>

namespace s2
{

class Actor;
class Sprite;
class SprTreePath;

class ActorFactory
{
public:
	Actor* Create(const SprTreePath& parent, const Sprite* child) const;

private:
	SINGLETON_DECLARATION(ActorFactory);

}; // ActorFactory

}

#endif // _SPRITE2_ACTOR_FACTORY_H_