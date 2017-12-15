#include "sprite2/DrawTask.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Actor.h"
#include "sprite2/Callback.h"

#include <cooking/DisplayList.h>
#include <multitask/TaskStat.h>

namespace s2
{

DrawTask::DrawTask(const ActorConstPtr& actor, const RenderParams& rp)
	: m_actor(actor)
	, m_rp(rp)
{
	DrawTaskMgr::Instance()->AddResult(this);
}

void DrawTask::Run()
{
#ifndef S2_DISABLE_DEFERRED
	DrawTaskMgr* task_mgr = DrawTaskMgr::Instance();

	mt::TaskStat::Checkpoint cp(std::this_thread::get_id(), task_mgr->GetTaskStatType());

	auto& dlist = std::const_pointer_cast<Actor>(m_actor)->GetDisplayList();

	int thread_idx = Callback::QueryThreadIdx(std::this_thread::get_id());
	dlist->SetThreadIdx(thread_idx);

	DrawNode::Draw(dlist.get(), m_actor->GetSprRaw(), m_rp);

	task_mgr->OneTaskFinished();
#endif // S2_DISABLE_DEFERRED
}

void DrawTask::Flush()
{
#ifndef S2_DISABLE_DEFERRED
	std::const_pointer_cast<Actor>(m_actor)->GetDisplayList()->Replay(-1, -1);
#endif // S2_DISABLE_DEFERRED
}

void DrawTask::Initialize(const ActorConstPtr& actor, const RenderParams& rp)
{
	m_actor = actor;
	memcpy(&m_rp, &rp, sizeof(rp));

	DrawTaskMgr::Instance()->AddResult(this);
}

void DrawTask::Terminate()
{
#ifndef S2_DISABLE_DEFERRED
	std::const_pointer_cast<Actor>(m_actor)->GetDisplayList()->Clear();
#endif // S2_DISABLE_DEFERRED
}

/************************************************************************/
/* class DrawTaskMgr                                                  */
/************************************************************************/

CU_SINGLETON_DEFINITION(DrawTaskMgr)

DrawTaskMgr::DrawTaskMgr()
	: m_working(0)
{
	m_task_stat_type = mt::TaskStat::Instance()->RegisterTaskType("draw");
}

DrawTask* DrawTaskMgr::Fetch(const ActorConstPtr& actor, const RenderParams& rp)
{
	++m_working;
	mt::Task* t = m_freelist.Front();
	DrawTask* tt = static_cast<DrawTask*>(t);
	if (!t) {
		tt = new DrawTask(actor, rp);
	} else {
		m_freelist.Pop();
		tt->Initialize(actor, rp);
	}
	return tt;
}

void DrawTaskMgr::AddResult(DrawTask* task)
{
	m_result.Push(task);
}

void DrawTaskMgr::Flush()
{
	while (mt::Task* t = m_result.TryPop())
	{
		DrawTask* tt = static_cast<DrawTask*>(t);
		tt->Flush();
		tt->Terminate();
		m_freelist.Push(t);
	}
}

}