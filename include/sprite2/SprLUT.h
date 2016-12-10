#ifndef _SPRITE2_SPR_LUT_H_
#define _SPRITE2_SPR_LUT_H_

#include <CU_Singleton.h>

#include <vector>

namespace s2
{

class Sprite;

class SprLUT
{
public:	
	void Insert(Sprite* spr);
	bool Delete(Sprite* spr);

	Sprite* Query(int id);

private:
	static const size_t HASH_SZ_TBL[];
	static const int HASH_SZ_TBL_SZ;

	static const int MAX_AVERAGE_SEARCH_LENGTH = 3;

private:
	void Rehash();
	
	int Hash(int spr_id) const;

private:
	std::vector<Sprite*>* m_hash;
	int m_hash_sz_idx;

	int m_search_length;
	int m_search_times;

	SINGLETON_DECLARATION(SprLUT);

}; // SprLUT

}

#endif // _SPRITE2_SPR_LUT_H_