#ifndef _SPRITE2_COMPLEX_ACTOR_H_
#define _SPRITE2_COMPLEX_ACTOR_H_

#include "sprite2/Actor.h"

namespace s2
{

class ComplexActor : public Actor
{
public:
	ComplexActor(const SprConstPtr& spr, const ActorConstPtr& parent);

	void SetAction(int action);
	int GetAction() const;

private:
	int m_action;

}; // ComplexActor

}

#endif // _SPRITE2_COMPLEX_ACTOR_H_