#ifndef _SPRITE2_SPR_TREE_PATH_INL_
#define _SPRITE2_SPR_TREE_PATH_INL_

namespace s2
{

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

}

#endif // _SPRITE2_SPR_TREE_PATH_INL_