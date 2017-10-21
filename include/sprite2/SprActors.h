#ifndef _SPRITE2_SPR_ACTORS_H_
#define _SPRITE2_SPR_ACTORS_H_

#include "s2_typedef.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <memory>

//#define	S2_SPR_ACTORS_QUERY_COUNT

namespace s2
{

class Actor;

class SprActors : private cu::Uncopyable
{
public:	
	void Add(const ActorPtr& actor);
	void Del(const ActorPtr& actor);

	Actor* Query(const Actor* prev) const;
	ActorPtr QueryPtr(const Actor* prev) const;

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
	CU_VEC<ActorPtr> m_actors;

#ifdef S2_SPR_ACTORS_QUERY_COUNT
	static int m_query_count;
#endif // S2_SPR_ACTORS_QUERY_COUNT

}; // SprActors

}

#include "SprActors.inl"

#endif // _SPRITE2_SPR_ACTORS_H_