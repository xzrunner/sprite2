#ifndef _SPRITE2_SPR_TREE_PATH_H_
#define _SPRITE2_SPR_TREE_PATH_H_

#include <iostream>

#define SPR_TREE_PATH_STATIC

namespace s2
{

struct STPBuffer;

class SprTreePath
{
public:
	SprTreePath();
	SprTreePath(const SprTreePath& path);
	SprTreePath& operator = (const SprTreePath& path);
	~SprTreePath();

	bool operator == (const SprTreePath& path) const;

	bool Empty() const;

	void Push(int id);
	void Pop();
	int Top() const;

	int GetVal() const { return m_val; }
 
private:
	friend std::ostream & operator << (std::ostream& os, const SprTreePath& path);

private:
	int m_val;

#ifdef SPR_TREE_PATH_STATIC
	static const int MAX_PATH = 16;
	int m_ids[MAX_PATH];
	int m_num;
#else
	STPBuffer* m_buf;
#endif // SPR_TREE_PATH_STATIC

}; // SprTreePath

}

#include "SprTreePath.inl"

#endif // _SPRITE2_SPR_TREE_PATH_H_