#ifndef _SPRITE2_SPR_TREE_PATH_H_
#define _SPRITE2_SPR_TREE_PATH_H_

#include "s2_config.h"

#include <iostream>

#define SPR_TREE_PATH_STATIC

#ifdef S2_SPR_TREE_DEBUG
#include <vector>
#include <stdint.h>
#endif // S2_SPR_TREE_DEBUG

namespace s2
{

struct STPBuffer;
class Sprite;

class SprTreePath
{
public:
	SprTreePath();
	SprTreePath(const SprTreePath& path);
	SprTreePath& operator = (const SprTreePath& path);
	~SprTreePath();

	bool operator == (const SprTreePath& path) const;

	bool Empty() const;

	void Push(int spr_id);
	void Push(const Sprite& spr);
	void Pop();
	int Top() const;

	void Clear();

	int GetVal() const { return m_val; }
 
private:
	friend std::ostream & operator << (std::ostream& os, const SprTreePath& path);

private:
	int m_val;

#ifdef SPR_TREE_PATH_STATIC
	static const int MAX_PATH_NUM = 16;
	int m_ids[MAX_PATH_NUM];
	int m_num;
#else
	STPBuffer* m_buf;
#endif // SPR_TREE_PATH_STATIC

#ifdef S2_SPR_TREE_DEBUG
	std::vector<std::string> m_name_paths;
	std::vector<uint32_t>    m_sym_paths;
#endif // S2_SPR_TREE_DEBUG

}; // SprTreePath

}

#include "SprTreePath.inl"

#endif // _SPRITE2_SPR_TREE_PATH_H_