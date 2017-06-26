#ifndef _SPRITE2_SPR_ACTORS_H_
#define _SPRITE2_SPR_ACTORS_H_

#define SPR_ACTORS_HASH

#include <CU_Uncopyable.h>

#include <vector>

#ifdef SPR_ACTORS_HASH
struct ds_hash;
#endif // SPR_ACTORS_HASH

namespace s2
{

class Actor;

class SprActors : private cu::Uncopyable
{
public:
	SprActors();
	~SprActors();
	
	void Add(Actor* actor);
	void Del(Actor* actor);

	const Actor* Query(const Actor* prev) const;

	bool IsEmpty() const;
	int Size() const;

	void Clear();

	void Connect(const Actor* prev);

	const std::vector<Actor*>& GetActors() const { return m_actors; }

private:
	std::vector<Actor*> m_actors;

#ifdef SPR_ACTORS_HASH
	struct ds_hash* m_hash;
#endif // SPR_ACTORS_HASH

}; // SprActors

}

#include "SprActors.inl"

#endif // _SPRITE2_SPR_ACTORS_H_