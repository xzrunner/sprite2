#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"

#include <assert.h>

namespace s2
{

CU_SINGLETON_DEFINITION(RenderTargetMgr);

const int RenderTargetMgr::WIDTH  = 1024;
const int RenderTargetMgr::HEIGHT = 1024;

static RenderTarget* (*FETCH_SCREEN)() = nullptr;
static void (*RETURN_SCREEN)(RenderTarget* rt) = nullptr;

RenderTargetMgr::RenderTargetMgr()
{
}

RenderTarget* RenderTargetMgr::Fetch()
{
	for (int i = 0, n = m_items.size(); i < n; ++i) 
	{
		if (!m_items[i].available) {
			continue;
		}
		m_items[i].available = false;
		assert(m_items[i].rt);
		return m_items[i].rt;
	}

	Item item;
	item.rt = new RenderTarget(WIDTH, HEIGHT);
	item.available = false;
	m_items.push_back(item);
	return item.rt;
}

void RenderTargetMgr::Return(RenderTarget* rt)
{
	if (!rt) {
		return;
	}

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (m_items[i].rt == rt) {
			m_items[i].available = true;
			return;
		}
	}
}

int RenderTargetMgr::GetTexID(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_items.size())) {
		return  -1;
	} else {
		return m_items[idx].rt->GetTexID();
	}
}

void RenderTargetMgr::InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt))
{
	FETCH_SCREEN = fetch_screen;
	RETURN_SCREEN = return_screen;
}

RenderTarget* RenderTargetMgr::FetchScreen()
{
	if (FETCH_SCREEN) {
		return FETCH_SCREEN();
	} else {
		return nullptr;
	}
}

void RenderTargetMgr::ReturnScreen(RenderTarget* rt)
{
	if (RETURN_SCREEN) {
		RETURN_SCREEN(rt);
	}
}

}