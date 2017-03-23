//#include "ActorLUT.h"
//#include "S2_Actor.h"
//
//#include <logger.h>
//
//namespace s2
//{
//
//SINGLETON_DEFINITION(ActorLUT);
//
//const size_t ActorLUT::HASH_SZ_TBL[] = {
//	11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51439, 102881, 202881, 402881
//};
//
//const int ActorLUT::HASH_SZ_TBL_SZ = sizeof(HASH_SZ_TBL) / sizeof(HASH_SZ_TBL[0]);
//
//ActorLUT::ActorLUT()
//	: m_search_length(0)
//	, m_search_times(0)
//	, m_count(0)
//{
//	m_hash_sz_idx = 0;
//	m_hash = new std::vector<Actor*>[HASH_SZ_TBL[m_hash_sz_idx]];
//}
//
//ActorLUT::~ActorLUT()
//{
//	delete[] m_hash;
//}
//
//void ActorLUT::Insert(Actor* actor)
//{
//	int idx = Hash(actor->GetTreePath());
//	m_hash[idx].push_back(actor);
//	++m_count;
//}
//
//bool ActorLUT::Delete(Actor* actor)
//{
//	if ((float)m_search_length / m_search_times > MAX_AVERAGE_SEARCH_LENGTH) {
//		Rehash();
//	}
//
//	++m_search_times;
//	int idx = Hash(actor->GetTreePath());
//	std::vector<Actor*>& list = m_hash[idx];
//	for (int i = 0, n = list.size(); i < n; ++i) 
//	{
//		++m_search_length;
//		if (list[i] == actor) {
//			list.erase(list.begin() + i);
//			--m_count;
//			return true;
//		}
//	}
//	return false;
//}
//
//Actor* ActorLUT::Query(const SprTreePath& path)
//{
//	if ((float)m_search_length / m_search_times > MAX_AVERAGE_SEARCH_LENGTH) {
//		Rehash();
//	}
//
//	++m_search_times;
//	int idx = Hash(path);
//	const std::vector<Actor*>& list = m_hash[idx];
//	for (int i = 0, n = list.size(); i < n; ++i) 
//	{
//		++m_search_length;
//		if (list[i]->GetTreePath() == path) {
//			return list[i];
//		}
//	}
//	return NULL;
//}
//
//void ActorLUT::Clear()
//{
//	m_hash_sz_idx = 0;
//	m_search_length = m_search_times = 0;
//	m_count = 0;
//
//	int sz = HASH_SZ_TBL[m_hash_sz_idx];
//	for (int i = 0; i < sz; ++i) {
//		m_hash[i].clear();
//	}
//}
//
//void ActorLUT::Rehash()
//{
//	if (m_hash_sz_idx + 1 >= HASH_SZ_TBL_SZ) {
//		LOGW("ActorLUT::Rehash error hash sz idx %d, %f\n", m_hash_sz_idx, (float)m_search_length / m_search_times);
//		return;
//	}
//
//	++m_hash_sz_idx;
//	std::vector<Actor*>* new_hash = new std::vector<Actor*>[HASH_SZ_TBL[m_hash_sz_idx]];
//	for (int i = 0, n = HASH_SZ_TBL[m_hash_sz_idx - 1]; i < n; ++i) {
//		const std::vector<Actor*>& list = m_hash[i];
//		for (int j = 0, m = list.size(); j < m; ++j) {
//			int idx = Hash(list[j]->GetTreePath());
//			new_hash[idx].push_back(list[j]);	
//		}
//	}
//
//	delete[] m_hash;
//	m_hash = new_hash;
//
//	m_search_length = m_search_times = 0;
//}
//
//int ActorLUT::Hash(const SprTreePath& path) const
//{
//	int val = path.GetVal();
//	return val % HASH_SZ_TBL[m_hash_sz_idx];
//}
//
//}