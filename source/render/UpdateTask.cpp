#include "sprite2/UpdateTask.h"
#include "sprite2/Actor.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/ProxyHelper.h"
#include "sprite2/SymType.h"
#include "sprite2/Symbol.h"

#include <multitask/TaskStat.h>

namespace s2
{

UpdateTask::UpdateTask(const ActorPtr& actor, bool force)
	: m_actor(actor)
	, m_force(force)
{
}

void UpdateTask::Run()
{
	mt::TaskStat::Checkpoint cp(std::this_thread::get_id(), 
		UpdateTaskMgr::Instance()->GetTaskStatType());

	auto s2_spr = const_cast<Sprite*>(m_actor->GetSprRaw());

	UpdateParams up(m_actor.get());
	up.SetPrevMat(GetActorWorldMat(m_actor->GetParent()));
	up.SetForce(m_force);

	int old_inherit_update = ProxyHelper::SprGetInheritUpdate(*s2_spr);
	if (old_inherit_update != -1) {
		ProxyHelper::SprSetInheritUpdate(*s2_spr, true);
		s2_spr->Update(up);
		ProxyHelper::SprSetInheritUpdate(*s2_spr, old_inherit_update == 1);
	} else {
		s2_spr->Update(up);
	}
}

void UpdateTask::Initialize(const ActorPtr& actor, bool force)
{
	m_actor = actor;
	m_force = force;
}

S2_MAT UpdateTask::GetActorWorldMat(const ActorPtr& actor)
{
	S2_MAT mt;
	auto curr = actor;
	while (curr) {
		if (curr->GetSprRaw()->GetSymbol()->Type() == SYM_MASK) {
			break;
		}
		mt = mt * curr->GetLocalMat() * curr->GetSprRaw()->GetLocalMat();
		curr = curr->GetParent();
	}
	return mt;
}

/************************************************************************/
/* class UpdateTaskMgr                                                  */
/************************************************************************/

CU_SINGLETON_DEFINITION(UpdateTaskMgr)

UpdateTaskMgr::UpdateTaskMgr()
{
	m_task_stat_type = mt::TaskStat::Instance()->RegisterTaskType("update");
}

UpdateTask* UpdateTaskMgr::Fetch(const ActorPtr& actor, bool force)
{
	mt::Task* t = m_freelist.Front();
	UpdateTask* tt = static_cast<UpdateTask*>(t);
	if (!t) {
		tt = new UpdateTask(actor, force);
	} else {
		m_freelist.Pop();
		tt->Initialize(actor, force);
	}
	return tt;
}

void UpdateTaskMgr::AddResult(UpdateTask* task)
{
	m_result.Push(task);
}

}