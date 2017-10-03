#ifndef _SPRITE2_SPR_TIMER_H_
#define _SPRITE2_SPR_TIMER_H_

#include <cu/cu_macro.h>

namespace s2
{

class SprTimer
{
public:
	void Init();

	bool Update(float dt);

	CU_SINGLETON_DECLARATION(SprTimer)

}; // SprTimer

}

#endif // _SPRITE2_SPR_TIMER_H_