#ifndef _SPRITE2_SPR_TREE_PATH_H_
#define _SPRITE2_SPR_TREE_PATH_H_

#include <vector>

namespace s2
{

class SprTreePath
{
public:
	SprTreePath();

	bool operator == (const SprTreePath& path) const;

	void Push(int id);
	void Pop();

	int GetVal() const { return m_val; }

private:
	friend std::ostream & operator << (std::ostream& os, const SprTreePath& path);

private:
	std::vector<int> m_ids;

	int m_val;

}; // SprTreePath

}

#include "SprTreePath.inl"

#endif // _SPRITE2_SPR_TREE_PATH_H_