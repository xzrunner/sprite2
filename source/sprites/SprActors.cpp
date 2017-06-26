#include "SprActors.h"

#include <ds_hash.h>

namespace s2
{

#ifdef SPR_ACTORS_HASH
static const int MAX_COUNT_NO_HASH = 4;
#endif // SPR_ACTORS_HASH

SprActors::SprActors()
#ifdef SPR_ACTORS_HASH
	: m_hash(NULL)
#endif // SPR_ACTORS_HASH
{
}

SprActors::~SprActors()
{
#ifdef SPR_ACTORS_HASH
	if (m_hash) {
		ds_hash_release(m_hash); 
	}
#endif // SPR_ACTORS_HASH
}

#ifdef SPR_ACTORS_HASH

static unsigned int
_hash_func(int hash_sz, void* key) 
{
	uint32_t k = reinterpret_cast<uint32_t>(key);
	return k % hash_sz;
}

static bool
_equal_func(void* key0, void* key1)
{
	return key0 == key1;
}

#endif // SPR_ACTORS_HASH

void SprActors::Add(Actor* actor)
{
	m_actors.push_back(actor);

#ifdef SPR_ACTORS_HASH
	if (m_hash) {
		ds_hash_insert(m_hash, const_cast<Actor*>(actor->GetParent()), actor, true);
	} else {
#endif // SPR_ACTORS_HASH
		if (m_actors.size() > MAX_COUNT_NO_HASH) {
			m_hash = ds_hash_create(MAX_COUNT_NO_HASH, MAX_COUNT_NO_HASH * 2, 0.5f, 
				_hash_func, _equal_func);
			for (int i = 0, n = m_actors.size(); i < n; ++i) {
				Actor* actor = m_actors[i];
				ds_hash_insert(m_hash, const_cast<Actor*>(actor->GetParent()), actor, true);
			}
		}
#ifdef SPR_ACTORS_HASH
	}
#endif // SPR_ACTORS_HASH
}

void SprActors::Del(Actor* actor)
{
	std::vector<Actor*>::iterator itr = m_actors.begin();
	for ( ; itr != m_actors.end(); ) {
		if (*itr == actor) {
			itr = m_actors.erase(itr);
		} else {
			++itr;
		}
	}

#ifdef SPR_ACTORS_HASH
	if (m_hash) {
		ds_hash_remove(m_hash, const_cast<Actor*>(actor->GetParent()));
	}
#endif // SPR_ACTORS_HASH
}

void SprActors::Clear()
{
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		delete m_actors[i];
	}
	m_actors.clear();

#ifdef SPR_ACTORS_HASH
	if (m_hash) {
		ds_hash_release(m_hash); 
		m_hash = NULL;
	}
#endif // SPR_ACTORS_HASH
}

void SprActors::Connect(const Actor* prev)
{
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		m_actors[i]->SetParent(prev);
	}

#ifdef SPR_ACTORS_HASH
	if (m_hash) 
	{
		ds_hash_clear(m_hash);
		for (int i = 0, n = m_actors.size(); i < n; ++i) {
			Actor* actor = m_actors[i];
			ds_hash_insert(m_hash, const_cast<Actor*>(actor->GetParent()), actor, true);
		}
	}
#endif // SPR_ACTORS_HASH
}

}