#include "SprLUT.h"
#include "S2_Sprite.h"

#include <logger.h>

namespace s2
{

SINGLETON_DEFINITION(SprLUT);

const size_t SprLUT::HASH_SZ_TBL[] = {
	11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51439, 102881, 202881, 402881
};

const int SprLUT::HASH_SZ_TBL_SZ = sizeof(HASH_SZ_TBL) / sizeof(HASH_SZ_TBL[0]);

SprLUT::SprLUT()
	: m_search_length(0)
	, m_search_times(0)
{
	m_hash_sz_idx = 0;
	m_hash = new std::vector<Sprite*>[HASH_SZ_TBL[m_hash_sz_idx]];
}

SprLUT::~SprLUT()
{
	delete[] m_hash;
}

void SprLUT::Insert(Sprite* spr)
{
	int idx = Hash(spr->GetID());
	m_hash[idx].push_back(spr);
}

bool SprLUT::Delete(Sprite* spr)
{
	if ((float)m_search_length / m_search_times > MAX_AVERAGE_SEARCH_LENGTH) {
		Rehash();
	}

	++m_search_times;
	int idx = Hash(spr->GetID());
	std::vector<Sprite*>& list = m_hash[idx];
	for (int i = 0, n = list.size(); i < n; ++i) 
	{
		++m_search_length;
		if (list[i] == spr) {
			list.erase(list.begin() + i);
			return true;
		}
	}
	return false;
}

Sprite* SprLUT::Query(int id)
{
	if ((float)m_search_length / m_search_times > MAX_AVERAGE_SEARCH_LENGTH) {
		Rehash();
	}

	++m_search_times;
	int idx = Hash(id);
	const std::vector<Sprite*>& list = m_hash[idx];
	for (int i = 0, n = list.size(); i < n; ++i) 
	{
		++m_search_length;
		if (list[i]->GetID() == id) {
			return list[i];
		}
	}
	return NULL;
}

void SprLUT::Rehash()
{
	if (m_hash_sz_idx + 1 >= HASH_SZ_TBL_SZ) {
		LOGW("SprLUT::Rehash error hash sz idx %d\n", m_hash_sz_idx);
		return;
	}

	++m_hash_sz_idx;
	std::vector<Sprite*>* new_hash = new std::vector<Sprite*>[HASH_SZ_TBL[m_hash_sz_idx]];
	for (int i = 0, n = HASH_SZ_TBL[m_hash_sz_idx - 1]; i < n; ++i) {
		const std::vector<Sprite*>& list = m_hash[i];
		for (int j = 0, m = list.size(); j < m; ++j) {
			int idx = Hash(list[j]->GetID());
			new_hash[idx].push_back(list[j]);	
		}
	}

	delete[] m_hash;
	m_hash = new_hash;
}

int SprLUT::Hash(int spr_id) const
{
	return spr_id % HASH_SZ_TBL[m_hash_sz_idx];
}

}