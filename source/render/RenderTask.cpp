#include "sprite2/RenderTask.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Actor.h"
#include "sprite2/Callback.h"

#include <cooking/DisplayList.h>

namespace s2
{

RenderTask::RenderTask(const ActorConstPtr& actor, const RenderParams& rp)
	: m_actor(actor)
	, m_rp(rp)
{
	RenderTaskMgr::Instance()->AddResult(this);
}

void RenderTask::Run()
{
#ifndef S2_DISABLE_DEFERRED
	auto& dlist = std::const_pointer_cast<Actor>(m_actor)->GetDisplayList();

	int thread_idx = Callback::QueryThreadIdx(std::this_thread::get_id());
	dlist->SetThreadIdx(thread_idx);

	DrawNode::Draw(dlist.get(), m_actor->GetSprRaw(), m_rp);

	RenderTaskMgr::Instance()->OneTaskFinished();
#endif // S2_DISABLE_DEFERRED
}

void RenderTask::Flush()
{
#ifndef S2_DISABLE_DEFERRED
	std::const_pointer_cast<Actor>(m_actor)->GetDisplayList()->Replay(-1, -1);
#endif // S2_DISABLE_DEFERRED
}

void RenderTask::Initialize(const ActorConstPtr& actor, const RenderParams& rp)
{
	m_actor = actor;
	memcpy(&m_rp, &rp, sizeof(rp));

	RenderTaskMgr::Instance()->AddResult(this);
}

void RenderTask::Terminate()
{
#ifndef S2_DISABLE_DEFERRED
	std::const_pointer_cast<Actor>(m_actor)->GetDisplayList()->Clear();
#endif // S2_DISABLE_DEFERRED
}

/************************************************************************/
/* class RenderTaskMgr                                                  */
/************************************************************************/

CU_SINGLETON_DEFINITION(RenderTaskMgr)

RenderTaskMgr::RenderTaskMgr()
	: m_working(0)
{
}

RenderTask* RenderTaskMgr::Fetch(const ActorConstPtr& actor, const RenderParams& rp)
{
	++m_working;
	mt::Task* t = m_freelist.Front();
	RenderTask* tt = static_cast<RenderTask*>(t);
	if (!t) {
		tt = new RenderTask(actor, rp);
	} else {
		m_freelist.Pop();
		tt->Initialize(actor, rp);
	}
	return tt;
}

void RenderTaskMgr::AddResult(RenderTask* task)
{
	m_result.Push(task);
}

void RenderTaskMgr::Flush()
{
	while (mt::Task* t = m_result.TryPop())
	{
		RenderTask* tt = static_cast<RenderTask*>(t);
		tt->Flush();
		tt->Terminate();
		m_freelist.Push(t);
	}
}

}