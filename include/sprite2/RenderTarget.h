#ifndef _SPRITE2_RENDER_TARGET_H_
#define _SPRITE2_RENDER_TARGET_H_

#include <CU_Singleton.h>

namespace ur { class RenderTarget; }

namespace s2
{

class RenderTarget
{
public:
	int  Fetch();
	void Return(int idx);

	void Bind(int idx);
	void Unbind(int idx);

	int  GetTexID(int idx);
	
public:
	static const int WIDTH  = 1024;
	static const int HEIGHT = 1024;

private:
	struct Item
	{
		ur::RenderTarget* rt;
		bool available;
	};
	
private:
	static const int MAX_COUNT = 4;

private:
	Item m_items[MAX_COUNT];

	SINGLETON_DECLARATION(RenderTarget);

}; // RenderTarget

}

#endif // _SPRITE2_RENDER_TARGET_H_