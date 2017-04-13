#include "ComplexActor.h"

namespace s2
{

ComplexActor::ComplexActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_action(-1)
{
}

void ComplexActor::SetAction(int action) 
{ 
	if (m_action == action) {
		return;
	}

	m_action = action;

	GetAABB().Update(this);
}

int ComplexActor::GetAction() const 
{ 
	return m_action; 
}

}