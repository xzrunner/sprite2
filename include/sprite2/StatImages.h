#ifndef _SPRITE2_STAT_IMAGES_H_
#define _SPRITE2_STAT_IMAGES_H_

#include <CU_Singleton.h>

#include <string>
#include <map>

namespace s2
{

class StatImages
{
public:
	void Add(int id, int width, int height, int type);
	void Remove(int id, int width, int height, int type);

	void Print(std::string& str) const;	

	const std::map<int, float>& GetID2Mem() const { return m_id2mem; }

public:
	static const int UNKNOWN_IMG_ID = -1;

private:
	static int GetTexBPP(int type);

private:
	int   m_count;
	float m_memory;
	int   m_4096_count;

	std::map<int, float> m_id2mem;

	SINGLETON_DECLARATION(StatImages);

}; // StatImages

}

#endif // _SPRITE2_STAT_IMAGES_H_