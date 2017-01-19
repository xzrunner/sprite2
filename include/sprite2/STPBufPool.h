#ifndef _SPRITE2_SPT_BUF_POOL_H_
#define _SPRITE2_SPT_BUF_POOL_H_

#include <CU_Singleton.h>

#include <vector>

#define STP_BUF_POOL_STAT

namespace s2
{

struct STPBuffer;

class STPBufPool
{
public:
	STPBuffer* Fetch(int cap);
	void Return(STPBuffer* buf);
	
private:
	static int GetIndex(int cap);

private:
	static const int MAX_LEVEL = 7;

private:
	// 2 4 8 16 ...
	std::vector<STPBuffer*> m_freelists;

#ifdef STP_BUF_POOL_STAT
	std::vector<int> m_list_num;
	std::vector<int> m_new_num;
	int m_tot_free;
	int m_tot_new;
#endif // STP_BUF_POOL_STAT

	SINGLETON_DECLARATION(STPBufPool)

}; // STPBufPool

}

#endif // _SPRITE2_SPT_BUF_POOL_H_