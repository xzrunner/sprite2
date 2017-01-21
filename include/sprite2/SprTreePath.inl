#ifndef _SPRITE2_SPR_TREE_PATH_INL_
#define _SPRITE2_SPR_TREE_PATH_INL_

#include "STPBuffer.h"

#include <assert.h>

namespace s2
{

inline
bool SprTreePath::operator == (const SprTreePath& path) const
{
#ifdef SPR_TREE_PATH_STATIC
	if (path.m_val != m_val || path.m_num != m_num) {
		return false;
	}
	if (m_val == 0 || m_num == 0) {
		return true;
	}
	return memcmp(m_ids, path.m_ids, sizeof(int) * m_num) == 0;
#else
	if (path.m_val != m_val || !path.m_buf || !m_buf || path.m_buf->num != m_buf->num) {
		return false;
	}
	if (m_val == 0 || m_buf->num == 0) {
		return true;
	}
	return memcmp(m_buf->ids, path.m_buf->ids, sizeof(int) * m_buf->num) == 0;
#endif // SPR_TREE_PATH_STATIC
}

inline
bool SprTreePath::Empty() const
{
#ifdef SPR_TREE_PATH_STATIC
	return m_num == 0;
#else
	return m_buf->num == 0;
#endif // SPR_TREE_PATH_STATIC
}

#ifdef SPR_TREE_PATH_STATIC
inline
void SprTreePath::Push(int id)
{
	assert(m_num < MAX_PATH);
	m_ids[m_num++] = id;
	m_val += id * m_num;
}
#endif // SPR_TREE_PATH_STATIC

inline
void SprTreePath::Pop() 
{ 
#ifdef SPR_TREE_PATH_STATIC
	if (m_num == 0) {
		return;
	}

	m_val -= m_ids[m_num - 1] * m_num;
	--m_num;
#else
	if (m_buf->num == 0) {
		return;
	}

	m_val -= m_buf->ids[m_buf->num - 1] * m_buf->num;
	--m_buf->num;
#endif // SPR_TREE_PATH_STATIC
}

inline
int SprTreePath::Top() const 
{
#ifdef SPR_TREE_PATH_STATIC
	return m_num == 0 ? -1 : m_ids[m_num - 1];
#else
	return m_buf->num == 0 ? -1 : m_buf->ids[m_buf->num - 1];
#endif // SPR_TREE_PATH_STATIC
}

}

#endif // _SPRITE2_SPR_TREE_PATH_INL_