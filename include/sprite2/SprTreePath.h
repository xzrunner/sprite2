#ifndef _SPRITE2_SPR_TREE_PATH_H_
#define _SPRITE2_SPR_TREE_PATH_H_

#include <vector>

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

	STPBuffer* m_buf;

}; // SprTreePath

}

#include "SprTreePath.inl"

#endif // _SPRITE2_SPR_TREE_PATH_H_