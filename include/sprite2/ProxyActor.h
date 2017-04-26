#ifndef _SPRITE2_PROXY_ACTOR_H_
#define _SPRITE2_PROXY_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class ProxyActor : public Actor
{
public:
	ProxyActor(const Sprite* spr, const Actor* parent);

}; // ProxyActor

}

#endif // _SPRITE2_PROXY_ACTOR_H_