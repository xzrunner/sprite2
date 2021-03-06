#ifndef _SPRITE2_DRAW_TASK_H_
#define _SPRITE2_DRAW_TASK_H_

#include "sprite2/typedef.h"
#include "sprite2/RenderParams.h"

#include <multitask/Task.h>
#include <cu/cu_macro.h>

#include <atomic>

namespace s2
{

class DrawTask : public mt::Task
{
public:
	DrawTask(const ActorConstPtr& actor, const RenderParams& rp);

	virtual void Run() override;

	void Flush();

	void Initialize(const ActorConstPtr& actor, const RenderParams& rp);
	void Terminate();
	
private:
	ActorConstPtr m_actor;
	RenderParams  m_rp;

}; // DrawTask

class DrawTaskMgr
{
public:
	DrawTask* Fetch(const ActorConstPtr& actor, const RenderParams& rp);

	void AddResult(DrawTask* task);

	bool IsAllTaskFinished() const { return m_working == 0; }
	void OneTaskFinished() { --m_working; }

	void Flush();

	int GetTaskStatType() const { return m_task_stat_type; }

private:
	mt::TaskQueue m_freelist;
	mt::SafeTaskQueue m_result;

	std::atomic<int> m_working;

	int m_task_stat_type;

	CU_SINGLETON_DECLARATION(DrawTaskMgr)

}; // DrawTaskMgr

}

#endif // _SPRITE2_DRAW_TASK_H_