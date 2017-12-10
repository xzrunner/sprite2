#include "sprite2/Callback.h"

namespace s2
{

static Callback::Funs FUNS;
	
void Callback::RegisterCallback(const Callback::Funs& funs)
{
	FUNS = funs;
}

void Callback::SubmitTask(mt::Task* task)
{
	FUNS.submit_task(task);
}

int Callback::QueryThreadIdx(std::thread::id id)
{
	return FUNS.query_thread_idx(id);
}

}

