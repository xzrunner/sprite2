#ifndef _SPRITE2_PARTICLE3D_ACTOR_H_
#define _SPRITE2_PARTICLE3D_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class Particle3dEmitter;

class Particle3dActor : public Actor
{
public:
	Particle3dActor(const Sprite* spr, const Actor* parent)
		: Actor(spr, parent) 
		, m_et(NULL)
	{}

private:
	Particle3dEmitter* m_et;

}; // Particle3dActor

}

#endif // _SPRITE2_PARTICLE3D_ACTOR_H_