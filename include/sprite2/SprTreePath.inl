#ifndef _SPRITE2_SPR_TREE_PATH_INL_
#define _SPRITE2_SPR_TREE_PATH_INL_

#include "STPBuffer.h"

#include <assert.h>

namespace s2
{

inline
bool SprTreePath::operator == (const SprTreePath& path) const
{
	if (path.m_val != m_val || !path.m_buf || !m_buf || path.m_buf->num != m_buf->num) {
		return false;
	}
	return memcmp(m_buf->ids, path.m_buf->ids, sizeof(int) * m_buf->num) == 0;
}

inline
bool SprTreePath::Empty() const
{
	return m_buf->num == 0;
}

inline
void SprTreePath::Pop() 
{ 
	if (m_buf->num == 0) {
		return;
	}

	m_val -= m_buf->ids[m_buf->num - 1] * m_buf->num;
	--m_buf->num;
}

inline
int SprTreePath::Top() const 
{
	return m_buf->num == 0 ? -1 : m_buf->ids[m_buf->num - 1];
}

}

#endif // _SPRITE2_SPR_TREE_PATH_INL_