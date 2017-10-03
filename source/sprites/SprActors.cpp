#include "SprActors.h"

namespace s2
{

SprActors::SprActors()
{
}

SprActors::~SprActors()
{
}

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
