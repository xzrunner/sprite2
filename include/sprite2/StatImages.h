#ifndef _SPRITE2_STAT_IMAGES_H_
#define _SPRITE2_STAT_IMAGES_H_

#include <CU_Singleton.h>

#include <string>

namespace s2
{

class StatImages
{
public:
	void Add(int width, int height, int type);
	void Remove(int width, int height, int type);

	void Print(std::string& str) const;	
	
private:
	static int GetTexBPP(int type);

private:
	int   m_count;
	float m_memory;
	int   m_4096_count;

	SINGLETON_DECLARATION(StatImages);

}; // StatImages

}

#endif // _SPRITE2_STAT_IMAGES_H_