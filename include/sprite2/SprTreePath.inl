#ifndef _SPRITE2_SPR_TREE_PATH_INL_
#define _SPRITE2_SPR_TREE_PATH_INL_

#include "S2_Sprite.h"

#include <assert.h>
#include <string.h>

namespace s2
{

inline
SprTreePath::SprTreePath()
	: m_val(0)
	, m_num(0)
{
}

inline
SprTreePath::SprTreePath(const SprTreePath& path)
	: m_val(path.m_val)
	, m_num(path.m_num)
{
	 memcpy(m_ids, path.m_ids, sizeof(int) * m_num);
}

inline
SprTreePath& SprTreePath::operator = (const SprTreePath& path)
{
	m_val = path.m_val;
	m_num = path.m_num;
	memcpy(m_ids, path.m_ids, sizeof(int) * m_num);
	return *this;
}

inline
bool SprTreePath::operator == (const SprTreePath& path) const
{
	if(path.m_val != m_val || path.m_num != m_num) {
		return false;
	}
	if(m_val == 0 || m_num == 0) {
		return true;
	}
	return memcmp(m_ids, path.m_ids, sizeof(int) * m_num) == 0;
}

inline
bool SprTreePath::IsPartOf(const SprTreePath& long_path) const
{
	if (m_num > long_path.m_num) {
		return false;
	}
	return memcmp(m_ids, long_path.m_ids, sizeof(int) * m_num) == 0;
}

inline
bool SprTreePath::Empty() const
{
	return m_num == 0;
}

inline
void SprTreePath::Push(int spr_id)
{
	assert(m_num < MAX_PATH_NUM);
	m_ids[m_num ++] = spr_id;
	m_val += spr_id * m_num;
}

inline
void SprTreePath::Pop()
{
	if (m_num == 0) {
		return;
	}

	m_val -= m_ids[m_num - 1] * m_num;
	--m_num;
}

inline
void SprTreePath::Clear()
{
	m_num = 0;
	m_val = 0;
}

inline
int SprTreePath::QueryByIndex(int idx) const
{
	if (idx < 0 || idx >= m_num) {
		return -1;
	} else {
		return m_ids[idx];
	}
}

}

#endif // _SPRITE2_SPR_TREE_PATH_INL_

