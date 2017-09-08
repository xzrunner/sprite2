#ifndef _SPRITE2_CALLBACK_H_
#define _SPRITE2_CALLBACK_H_

namespace mt { class Task; }

namespace s2
{

class Callback
{
public:
	struct Funs
	{
		void (*submit_task)(mt::Task*);
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void SubmitTask(mt::Task*);

}; // Callback

}

#endif // _SPRITE2_CALLBACK_H_