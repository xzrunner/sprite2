#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(RenderTargetMgr);

const int RenderTargetMgr::WIDTH  = 1024;
const int RenderTargetMgr::HEIGHT = 1024;

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
	if (idx < 0 || idx >= m_items.size()) {
		return  -1;
	} else {
		return m_items[idx].rt->GetTexID();
	}
}

}