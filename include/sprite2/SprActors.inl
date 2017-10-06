#ifndef _SPRITE2_SPR_ACTORS_INL_
#define _SPRITE2_SPR_ACTORS_INL_

#include "S2_Actor.h"

namespace s2
{

inline
ActorPtr SprActors::Query(const Actor* prev) const
{
	for (auto& actor : m_actors) 
	{
		if (actor) {
			auto sp_p = actor->GetParent();
			if (sp_p && sp_p.get() == prev) {
				return actor;
			}
		}
	}
	return nullptr;
}

inline
bool SprActors::IsEmpty() const
{
	return m_actors.empty();
}

inline
int SprActors::Size() const
{
	return m_actors.size();
}

}

#endif // _SPRITE2_SPR_ACTORS_INL_