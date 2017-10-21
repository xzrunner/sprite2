#include "SprActors.h"

namespace s2
{

#ifdef S2_SPR_ACTORS_QUERY_COUNT
int SprActors::m_query_count = 0;
#endif // S2_SPR_ACTORS_QUERY_COUNT

void SprActors::Add(const ActorPtr& actor)
{
	m_actors.push_back(actor);
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
}

Actor* SprActors::Query(const Actor* prev) const
{
	return QueryPtr(prev).get();
}

ActorPtr SprActors::QueryPtr(const Actor* prev) const
{
#ifdef S2_SPR_ACTORS_QUERY_COUNT
	++m_query_count;
#endif // S2_SPR_ACTORS_QUERY_COUNT

	if (m_actors.empty()) {
		return nullptr;
	}

	const ActorPtr* ptr = &m_actors[0];
	for (int i = 0, n = m_actors.size(); i < n; ++i, ++ptr)
	{
		const ActorPtr& actor = *ptr;
		if (actor) {
			auto sp_p = actor->GetParent();
			if (sp_p) {
				if (sp_p.get() == prev) {
					return actor;
				}
			} else {
				if (!prev) {
					return actor;
				}
			}
		}
	}
	return nullptr;
}

void SprActors::Clear()
{
	m_actors.clear();
}

void SprActors::Connect(const ActorPtr& prev)
{
	for (auto& actor : m_actors) {
		if (actor) {
			actor->SetParent(prev);
		}
	}
}

}
