#ifndef _SPRITE2_SPR_TREE_PATH_INL_
#define _SPRITE2_SPR_TREE_PATH_INL_

namespace s2
{

inline
bool SprTreePath::Empty() const
{
	return m_ids.empty();
}

inline
void SprTreePath::Push(int id) 
{ 
	m_ids.push_back(id);
	m_val += id;
}

inline
void SprTreePath::Pop() 
{ 
	if (m_ids.empty()) {
		return;
	}

	m_val -= m_ids.back();
	m_ids.pop_back();
}

inline
int SprTreePath::Top() const 
{
	return m_ids.empty() ? -1 : m_ids.back();
}

}

#endif // _SPRITE2_SPR_TREE_PATH_INL_