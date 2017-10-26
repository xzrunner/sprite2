#ifndef _SPRITE2_SPR_ACTORS_H_
#define _SPRITE2_SPR_ACTORS_H_

#include "sprite2/typedef.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <memory>

//#define S2_SPR_ACTORS_QUERY_COUNT
#define S2_SPR_ACTORS_HASH

#ifdef S2_SPR_ACTORS_HASH
struct ds_hash;
#endif // S2_SPR_ACTORS_HASH

namespace s2
{

class Actor;

class SprActors : private cu::Uncopyable
{
public:	
#ifdef S2_SPR_ACTORS_HASH
	SprActors();
	~SprActors();
#endif // S2_SPR_ACTORS_HASH

	void Add(const ActorPtr& actor);
	void Del(const ActorPtr& actor);

	Actor* Query(const Actor* prev) const;
	const ActorPtr* QueryPtr(const Actor* prev) const;

	bool IsEmpty() const;
	int Size() const;

	void Clear();

	void Connect(const ActorPtr& prev);

	const auto& GetActors() const { return m_actors; }

#ifdef S2_SPR_ACTORS_QUERY_COUNT
	static int GetCount() { return m_query_count; }
	static void ResetCount() { m_query_count = 0; }
#endif // S2_SPR_ACTORS_QUERY_COUNT

private:
	int QueryFromVec(const Actor* prev) const;

private:
	CU_VEC<ActorPtr> m_actors;

#ifdef S2_SPR_ACTORS_HASH
	struct ds_hash* m_hash;
#endif // S2_SPR_ACTORS_HASH

#ifdef S2_SPR_ACTORS_QUERY_COUNT
	static int m_query_count;
#endif // S2_SPR_ACTORS_QUERY_COUNT

}; // SprActors

}

#include "sprite2/SprActors.inl"

#endif // _SPRITE2_SPR_ACTORS_H_