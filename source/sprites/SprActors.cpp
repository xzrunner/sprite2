#include "sprite2/SprActors.h"

namespace s2
{

#ifdef S2_SPR_ACTORS_QUERY_COUNT
int SprActors::m_query_count = 0;
#endif // S2_SPR_ACTORS_QUERY_COUNT

#ifdef S2_SPR_ACTORS_HASH
static const int MAX_COUNT_NO_HASH = 4;
#endif // S2_SPR_ACTORS_HASH

#ifdef S2_SPR_ACTORS_HASH
SprActors::SprActors()
	: m_hash(nullptr)
{
}

SprActors::~SprActors()
{
	if (m_hash) {
		ds_hash_release(m_hash);
	}
}
#endif // S2_SPR_ACTORS_HASH

#ifdef S2_SPR_ACTORS_HASH

static unsigned int
_hash_func(int hash_sz, void* key)
{
	uint64_t k = reinterpret_cast<uint64_t>(key);
	return k % hash_sz;
}

static bool
_equal_func(void* key0, void* key1)
{
	return key0 == key1;
}

#endif // S2_SPR_ACTORS_HASH

void SprActors::Add(const ActorPtr& actor)
{
	m_actors.push_back(actor);

#ifdef S2_SPR_ACTORS_HASH
	int idx = m_actors.size() - 1;
	if (m_hash) {
		ds_hash_insert(m_hash, actor->GetParentRaw(), reinterpret_cast<void*>(idx + 1), true);
	} else {
		if (m_actors.size() > MAX_COUNT_NO_HASH) {
			m_hash = ds_hash_create(MAX_COUNT_NO_HASH, MAX_COUNT_NO_HASH * 2, 0.5f,
				_hash_func, _equal_func);
			for (int i = 0, n = m_actors.size(); i < n; ++i) {
				auto& actor = m_actors[i];
				ds_hash_insert(m_hash, actor->GetParentRaw(), reinterpret_cast<void*>(i + 1), true);
			}
		}
	}
#endif // S2_SPR_ACTORS_HASH
}

void SprActors::Del(const ActorPtr& actor)
{
	auto itr = m_actors.begin();
	for ( ; itr != m_actors.end(); ) {
		if (*itr == actor) {
			itr = m_actors.erase(itr);
		} else {
			++itr;
		}
	}

#ifdef S2_SPR_ACTORS_HASH
	if (m_hash) {
		ds_hash_remove(m_hash, actor->GetParentRaw());
	}
#endif // S2_SPR_ACTORS_HASH
}

void SprActors::Clear()
{
	m_actors.clear();

#ifdef S2_SPR_ACTORS_HASH
	if (m_hash) {
		ds_hash_release(m_hash);
		m_hash = NULL;
	}
#endif // S2_SPR_ACTORS_HASH
}

void SprActors::Connect(const ActorPtr& prev)
{
	for (auto& actor : m_actors) {
		if (actor) {
			actor->SetParent(prev);
		}
	}

#ifdef S2_SPR_ACTORS_HASH
	if (m_hash)
	{
		ds_hash_clear(m_hash);
		for (int i = 0, n = m_actors.size(); i < n; ++i) {
			auto& actor = m_actors[i];
			ds_hash_insert(m_hash, actor->GetParentRaw(), reinterpret_cast<void*>(i + 1), true);
		}
	}
#endif // S2_SPR_ACTORS_HASH
}

int SprActors::QueryFromVec(const Actor* prev) const
{
#ifdef S2_SPR_ACTORS_QUERY_COUNT
	++m_query_count;
#endif // S2_SPR_ACTORS_QUERY_COUNT

	if (m_actors.empty()) {
		return -1;
	}

	const ActorPtr* ptr = &m_actors[0];
	for (int i = 0, n = m_actors.size(); i < n; ++i, ++ptr)
	{
		const ActorPtr& actor = *ptr;
		if (actor && actor->GetParentRaw() == prev) {
			return i;
		}
	}
	return -1;
}

}
