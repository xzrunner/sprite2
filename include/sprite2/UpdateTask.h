#ifndef _SPRITE2_UPDATE_TASK_H_
#define _SPRITE2_UPDATE_TASK_H_

#include "sprite2/typedef.h"
#include "sprite2/pre_defined.h"
#include S2_MAT_HEADER

#include <multitask/Task.h>
#include <cu/cu_macro.h>

namespace s2
{

class UpdateTask : public mt::Task
{
public:
	UpdateTask(const ActorPtr& actor, bool force);

	virtual void Run() override;

	void Initialize(const ActorPtr& actor, bool force);

private:
	static S2_MAT GetActorWorldMat(const ActorPtr& actor);

private:
	ActorPtr m_actor;

	bool m_force;

}; // UpdateTask

class UpdateTaskMgr
{
public:
	UpdateTask* Fetch(const ActorPtr& actor, bool force);

	void AddResult(UpdateTask* task);

	int GetTaskStatType() const { return m_task_stat_type; }

private:
	mt::TaskQueue m_freelist;
	mt::SafeTaskQueue m_result;

	int m_task_stat_type;

	CU_SINGLETON_DECLARATION(UpdateTaskMgr)

}; // UpdateTaskMgr

}

#endif // _SPRITE2_UPDATE_TASK_H_