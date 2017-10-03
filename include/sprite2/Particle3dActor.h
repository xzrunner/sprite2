#ifndef _SPRITE2_PARTICLE3D_ACTOR_H_
#define _SPRITE2_PARTICLE3D_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class Particle3dEmitter;

class Particle3dActor : public Actor
{
public:
	Particle3dActor(const SprConstPtr& spr, const ActorConstPtr& parent);
	virtual ~Particle3dActor();

	const std::shared_ptr<Particle3dEmitter>& GetEmitter() const { return m_et; }
	std::shared_ptr<Particle3dEmitter>& GetEmitter() { return m_et; }

private:
	std::shared_ptr<Particle3dEmitter> m_et;

}; // Particle3dActor

}

#endif // _SPRITE2_PARTICLE3D_ACTOR_H_