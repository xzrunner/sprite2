#ifndef _SPRITE2_SPR_ACTORS_H_
#define _SPRITE2_SPR_ACTORS_H_

#include "s2_typedef.h"

#include <cu/uncopyable.h>

#include <vector>
#include <memory>

namespace s2
{

class Actor;

class SprActors : private cu::Uncopyable
{
public:
	SprActors();
	~SprActors();
	
	void Add(const ActorPtr& actor);
	void Del(const ActorPtr& actor);

	Actor* Query(const Actor* prev) const;
	ActorPtr QueryPtr(const Actor* prev) const;

	bool IsEmpty() const;
	int Size() const;

	void Clear();

	void Connect(const ActorPtr& prev);

	const std::vector<ActorPtr>& GetActors() const { return m_actors; }

private:
	std::vector<ActorPtr> m_actors;

}; // SprActors

}

#include "SprActors.inl"

#endif // _SPRITE2_SPR_ACTORS_H_