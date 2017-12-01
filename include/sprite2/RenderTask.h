#ifndef _SPRITE2_RENDER_TASK_H_
#define _SPRITE2_RENDER_TASK_H_

#include "sprite2/typedef.h"
#include "sprite2/RenderParams.h"

#include <multitask/Task.h>
#include <cu/cu_macro.h>

#include <atomic>

namespace s2
{

class RenderTask : public mt::Task
{
public:
	RenderTask(const ActorConstPtr& actor, const RenderParams& rp);

	virtual void Run() override;

	void Flush();

	void Initialize(const ActorConstPtr& actor, const RenderParams& rp);
//	void Terminate();

private:
	ActorConstPtr m_actor;
	RenderParams  m_rp;

}; // RenderTask

class RenderTaskMgr
{
public:
	RenderTask* Fetch(const ActorConstPtr& actor, const RenderParams& rp);

	void AddResult(RenderTask* task);

	bool IsAllTaskFinished() const { return m_working == 0; }
	void OneTaskFinished() { --m_working; }

	void Flush();

private:
	mt::TaskQueue m_freelist;
	mt::SafeTaskQueue m_result;

	std::atomic<int> m_working;

	CU_SINGLETON_DECLARATION(RenderTaskMgr)

}; // RenderTaskMgr

}

#endif // _SPRITE2_RENDER_TASK_H_