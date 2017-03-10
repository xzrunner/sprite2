#ifndef _SPRITE2_COMPLEX_ACTOR_H_
#define _SPRITE2_COMPLEX_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class ComplexActor : public Actor
{
public:
	ComplexActor(const Sprite* spr, const SprTreePath& path)
		: Actor(spr, path) {}

	void SetAction(int action) { m_action = action; }
	int GetAction() const { return m_action; }

private:
	int m_action;

}; // ComplexActor

}

#endif // _SPRITE2_COMPLEX_ACTOR_H_