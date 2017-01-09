#include "RenderTargetMgr.h"
#include "RenderTarget.h"

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(RenderTargetMgr);

RenderTargetMgr::RenderTargetMgr()
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		m_items[i].rt = new RenderTarget(WIDTH, HEIGHT);
		m_items[i].available = true;
	}
}

RenderTarget* RenderTargetMgr::Fetch()
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		if (m_items[i].available) {
			m_items[i].available = false;
			return m_items[i].rt;
		}
	}
	return NULL;
}

void RenderTargetMgr::Return(RenderTarget* rt)
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		if (m_items[i].rt == rt) {
			m_items[i].available = true;
			return;
		}
	}
	assert(0);
}

int RenderTargetMgr::GetTexID(int idx) const
{
	if (idx < 0 || idx >= MAX_COUNT) {
		return  -1;
	} else {
		return m_items[idx].rt->GetTexID();
	}
}

}