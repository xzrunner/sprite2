#ifndef _SPRITE2_CALLBACK_H_
#define _SPRITE2_CALLBACK_H_

#include <thread>

namespace mt { class Task; }

namespace s2
{

class Callback
{
public:
	struct Funs
	{
		void (*submit_task)(mt::Task*);
		int  (*query_thread_idx)(std::thread::id id);
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void SubmitTask(mt::Task*);

	static int  QueryThreadIdx(std::thread::id id);

}; // Callback

}

#endif // _SPRITE2_CALLBACK_H_