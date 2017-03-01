#ifndef _SPRITE2_RENDER_TARGET_MGR_H_
#define _SPRITE2_RENDER_TARGET_MGR_H_

#include <CU_Singleton.h>

#include <vector>

namespace s2
{

class RenderTarget;

class RenderTargetMgr
{
public:
	RenderTarget* Fetch();
	void Return(RenderTarget* rt);

	int GetTexID(int idx) const;

public:
	static const int WIDTH  = 1024;
	static const int HEIGHT = 1024;

private:
	struct Item
	{
		RenderTarget* rt;
		bool available;

		Item() : rt(NULL), available(true) {}
	};
	
private:
	std::vector<Item> m_items;

	SINGLETON_DECLARATION(RenderTargetMgr);

}; // RenderTargetMgr

}

#endif // _SPRITE2_RENDER_TARGET_MGR_H_