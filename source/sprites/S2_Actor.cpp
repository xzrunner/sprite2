#include "S2_Actor.h"
#include "S2_Sprite.h"

namespace s2
{

Actor::Actor(const Sprite* spr, const SprTreePath& path)
	: m_spr(spr)
	, m_path(path)
{
	if (m_spr) {
		m_spr->AddActor(this);
	}
}

}