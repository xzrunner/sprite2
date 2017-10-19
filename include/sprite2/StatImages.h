#ifndef _SPRITE2_STAT_IMAGES_H_
#define _SPRITE2_STAT_IMAGES_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

class StatImages
{
public:
	void Add(int id, int width, int height, int type);
	void Remove(int id, int width, int height, int type);

	void Print(CU_STR& str) const;	

	const CU_MAP<int, float>& GetID2Mem() const { return m_id2mem; }

public:
	static const int UNKNOWN_IMG_ID = -1;

private:
	static int GetTexBPP(int type);

private:
	int   m_count;
	float m_memory;
	int   m_4096_count;

	CU_MAP<int, float> m_id2mem;

	CU_SINGLETON_DECLARATION(StatImages);

}; // StatImages

}

#endif // _SPRITE2_STAT_IMAGES_H_