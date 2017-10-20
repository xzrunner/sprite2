#include "SprActors.h"

namespace s2
{

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
	if (m_actors.empty()) {
		return nullptr;
	}

	const ActorPtr* ptr = &m_actors[0];
	for (int i = 0, n = m_actors.size(); i < n; ++i, ++ptr)
	{
		const ActorPtr& actor = *ptr;
		if (actor) {
			auto sp_p = actor->GetParent();
			if (sp_p && sp_p.get() == prev) {
				return actor.get();
			}
		}
	}
	return nullptr;
}

ActorPtr SprActors::QueryPtr(const Actor* prev) const
{
	if (m_actors.empty()) {
		return nullptr;
	}

	const ActorPtr* ptr = &m_actors[0];
	for (int i = 0, n = m_actors.size(); i < n; ++i, ++ptr)
	{
		const ActorPtr& actor = *ptr;
		if (actor) {
			auto sp_p = actor->GetParent();
			if (sp_p && sp_p.get() == prev) {
				return actor;
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
