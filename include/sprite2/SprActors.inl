#ifndef _SPRITE2_SPR_ACTORS_INL_
#define _SPRITE2_SPR_ACTORS_INL_

#include "S2_Actor.h"

#ifdef S2_SPR_ACTORS_HASH
#include <ds_hash.h>
#endif // S2_SPR_ACTORS_HASH

namespace s2
{

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

inline
Actor* SprActors::Query(const Actor* prev) const
{
	const ActorPtr* ptr = QueryPtr(prev);
	return ptr ? ptr->get() : nullptr;
}

#ifdef S2_SPR_ACTORS_HASH

inline
const ActorPtr* SprActors::QueryPtr(const Actor* prev) const
{
	int idx = -1;
	if (m_hash) {
		void* val = ds_hash_query(m_hash, const_cast<Actor*>(prev));
		size_t v = reinterpret_cast<size_t>(val);
		if (v > 0) {
			idx = v - 1;
			assert(idx >= 0 && static_cast<size_t>(idx) < m_actors.size());
		}
	} else {
		idx = QueryFromVec(prev);
	}
	return idx < 0 ? nullptr : &m_actors[idx];
}

#else

inline
const ActorPtr* SprActors::QueryPtr(const Actor* prev) const
{
	int idx = QueryFromVec(prev);
	return idx < 0 ? nullptr : &m_actors[idx];
}

#endif // S2_SPR_ACTORS_HASH

}

#endif // _SPRITE2_SPR_ACTORS_INL_