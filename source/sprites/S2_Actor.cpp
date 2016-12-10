#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "ActorLUT.h"

namespace s2
{

Actor::Actor(Sprite* spr, const SprTreePath& path)
	: m_spr(spr)
	, m_path(path)
{
	if (m_spr) {
		m_spr->AddReference();
		m_spr->AddActor(this);
	}
}

Actor::~Actor()
{
	if (m_spr) {
		m_spr->RemoveActor(this);
		m_spr->RemoveReference();
	}
	ActorLUT::Instance()->Delete(this);
}

}