#ifndef _SPRITE2_RENDER_TARGET_MGR_H_
#define _SPRITE2_RENDER_TARGET_MGR_H_

#include <cu/cu_macro.h>

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

	void InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt));
	RenderTarget* FetchScreen();
	void ReturnScreen(RenderTarget* rt);

public:
	static const int WIDTH;
	static const int HEIGHT;

private:
	struct Item
	{
		RenderTarget* rt;
		bool available;

		Item() : rt(nullptr), available(true) {}
	};
	
private:
	std::vector<Item> m_items;

	CU_SINGLETON_DECLARATION(RenderTargetMgr);

}; // RenderTargetMgr

}

#endif // _SPRITE2_RENDER_TARGET_MGR_H_