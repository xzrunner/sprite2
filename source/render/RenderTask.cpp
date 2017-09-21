#include "sprite2/RenderTask.h"
#include "sprite2/DrawNodeDeferred.h"

#include <multitask/ThreadPool.h>

namespace s2
{

mt::TaskQueue RenderTask::m_freelist;

RenderTask::RenderTask(const Sprite* spr, RenderParams* rp)
	: m_spr(spr)
	, m_rp(rp)
{
}

void RenderTask::Run()
{
	DrawNodeDeferred::Draw(&m_dlist, m_spr, *m_rp);
	cooking::RenderTaskMgr::Instance()->AddResult(this);
}

void RenderTask::Flush()
{
	m_dlist.Replay();
//	m_dlist.Clear();
	RenderParamsPool::Instance()->Push(m_rp); 
	m_freelist.Push(this);
}

RenderTask* RenderTask::Create(const Sprite* spr, RenderParams* rp)
{
	cooking::RenderTaskMgr::Instance()->AddCount();

	mt::Task* t = m_freelist.Front();
	RenderTask* tt = static_cast<RenderTask*>(t);
	if (!t) {
		tt = new RenderTask(spr, rp);
	} else {
		m_freelist.Pop();
		tt->m_spr = spr;
		tt->m_rp = rp;
	}
	return tt;
}

}