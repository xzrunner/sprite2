#ifndef _SPRITE2_SPR_TREE_PATH_H_
#define _SPRITE2_SPR_TREE_PATH_H_

#include <vector>

namespace s2
{

class SprTreePath
{
public:
	SprTreePath();
	SprTreePath(const SprTreePath& path);
	SprTreePath& operator = (const SprTreePath& path);

	bool operator == (const SprTreePath& path) const;
	bool IsPartOf(const SprTreePath& long_path) const;

	bool Empty() const;

	void Push(int spr_id);
	void Pop();

	void Clear();

	int QueryByIndex(int idx) const;

private:
	int m_val;

	static const int MAX_PATH_NUM = 20;
	int m_ids[MAX_PATH_NUM];
	int m_num;

}; // SprTreePath

}

#include "SprTreePath.inl"

#endif // _SPRITE2_SPR_TREE_PATH_H_