#ifndef _SPRITE2_ACTOR_LUT_H_
#define _SPRITE2_ACTOR_LUT_H_

#include <CU_Singleton.h>

#include <vector>

namespace s2
{

class Actor;
class SprTreePath;

class ActorLUT
{
public:	
	void Insert(Actor* actor);
	bool Delete(Actor* actor);

	Actor* Query(const SprTreePath& path);

	int Count() const { return m_count; }

	void Clear();

private:
	static const size_t HASH_SZ_TBL[];
	static const int HASH_SZ_TBL_SZ;

	static const int MAX_AVERAGE_SEARCH_LENGTH = 3;

private:
	void Rehash();
	
	int Hash(const SprTreePath& path) const;

private:
	std::vector<Actor*>* m_hash;
	int m_hash_sz_idx;

	int m_search_length;
	int m_search_times;

	int m_count;

	SINGLETON_DECLARATION(ActorLUT);

}; // ActorLUT

}

#endif // _SPRITE2_ACTOR_LUT_H_