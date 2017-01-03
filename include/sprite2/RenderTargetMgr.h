#ifndef _SPRITE2_RENDER_TARGET_MGR_H_
#define _SPRITE2_RENDER_TARGET_MGR_H_

#include <CU_Singleton.h>

namespace s2
{

class RenderTarget;

class RenderTargetMgr
{
public:
	RenderTarget* Fetch();
	void Return(RenderTarget* rt);

public:
	static const int WIDTH  = 1024;
	static const int HEIGHT = 1024;

private:
	struct Item
	{
		RenderTarget* rt;
		bool available;
	};
	
private:
	static const int MAX_COUNT = 4;

private:
	Item m_items[MAX_COUNT];

	SINGLETON_DECLARATION(RenderTargetMgr);

}; // RenderTargetMgr

}

#endif // _SPRITE2_RENDER_TARGET_MGR_H_