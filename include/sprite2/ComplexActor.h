#ifndef _SPRITE2_COMPLEX_ACTOR_H_
#define _SPRITE2_COMPLEX_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class ComplexActor : public Actor
{
public:
	ComplexActor(const Sprite* spr, const Actor* parent);

	void SetAction(int action);
	int GetAction() const;

private:
	int m_action;

}; // ComplexActor

}

#endif // _SPRITE2_COMPLEX_ACTOR_H_