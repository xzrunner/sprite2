#include "STPBufPool.h"
#include "STPBuffer.h"

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(STPBufPool)

STPBufPool::STPBufPool()
{
	m_freelists.resize(MAX_LEVEL);
#ifdef STP_BUF_POOL_STAT
	m_list_num.resize(MAX_LEVEL, 0);
	m_new_num.resize(MAX_LEVEL, 0);
	m_tot_free = m_tot_new = 0;
#endif // STP_BUF_POOL_STAT
}

STPBuffer* STPBufPool::Fetch(int cap)
{
	int idx = GetIndex(cap);
	if (!m_freelists[idx]) {
#ifdef STP_BUF_POOL_STAT
		m_tot_new++;
		++m_new_num[idx];
#endif // STP_BUF_POOL_STAT
		return new STPBuffer(cap);
	}

#ifdef STP_BUF_POOL_STAT
	--m_tot_free;
	--m_list_num[idx];
#endif // STP_BUF_POOL_STAT

	STPBuffer* head = m_freelists[idx];
	m_freelists[idx] = head->next ? head->next : NULL;
	head->next = NULL;
	head->num = 0;
	return head;
}

void STPBufPool::Return(STPBuffer* buf)
{
	int idx = GetIndex(buf->cap);
	assert(idx < m_freelists.size());
	if (m_freelists[idx]) {
		buf->next = m_freelists[idx];
	} else {
		buf->next = NULL;
	}
	m_freelists[idx] = buf;

#ifdef STP_BUF_POOL_STAT
	++m_tot_free;
	++m_list_num[idx];
#endif // STP_BUF_POOL_STAT
}

int STPBufPool::GetIndex(int cap)
{
	switch (cap)
	{
	case 2:
		return 0;
	case 4:
		return 1;
	case 8:
		return 2;
	case 16:
		return 3;
	case 32:
		return 4;
	case 64:
		return 5;
	case 128:
		return 6;
	default:
		assert(0);
		return -1;
	}
}

}