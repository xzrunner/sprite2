#ifndef _SPRITE2_SPR_ACTORS_INL_
#define _SPRITE2_SPR_ACTORS_INL_

#include "S2_Actor.h"

#include <ds_hash.h>

namespace s2
{

inline
const Actor* SprActors::Query(const Actor* prev) const
{
	//static int tot = 0;
	//static int num = 0;
	//static std::map<const Sprite*, int> test;
	//std::map<const Sprite*, int>::iterator itr = test.find(this);
	//if (itr != test.end()) {
	//	if (m_actors.size() != itr->second) {
	//		tot += m_actors.size() - itr->second;
	//		itr->second = m_actors.size();
	//	}
	//} else {
	//	if (!m_actors.empty()) {
	//		test.insert(std::make_pair(this, m_actors.size()));
	//		tot += m_actors.size();
	//		++num;
	//	}
	//}
	//float avg = (float)(tot) / num;

	//////////////////////////////////////////////////////////////////////////

#ifdef SPR_ACTORS_HASH
	if (m_hash) {
		void* val = ds_hash_query(m_hash, const_cast<Actor*>(prev));
		return static_cast<const Actor*>(val);
	} else {
#endif // SPR_ACTORS_HASH
		for (int i = 0, n = m_actors.size(); i < n; ++i) {
			if (m_actors[i]->GetParent() == prev) {
				return m_actors[i];
			}
		}
		return NULL;
#ifdef SPR_ACTORS_HASH
	}
#endif // SPR_ACTORS_HASH
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