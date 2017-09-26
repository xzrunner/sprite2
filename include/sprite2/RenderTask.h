#ifndef _SPRITE2_RENDER_TASK_H_
#define _SPRITE2_RENDER_TASK_H_

#include <cooking/DisplayList.h>
#include <cooking/RenderTask.h>

namespace s2
{

class Sprite;
class RenderParams;

class RenderTask : public cooking::RenderTask
{
public:
	virtual void Run();
	virtual void Flush();
	
//	cooking::DisplayList& GetDisplayList() { return m_dlist; }

	static RenderTask* Create(const Sprite* spr, RenderParams* rp);

private:
	RenderTask(const Sprite* spr, RenderParams* rp);
	virtual ~RenderTask() {}

private:
//	cooking::DisplayList m_dlist;

	const Sprite* m_spr;
	RenderParams* m_rp;

	static mt::TaskQueue m_freelist;

}; // RenderTask

}

#endif // _SPRITE2_RENDER_TASK_H_