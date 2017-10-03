#ifndef _SPRITE2_TRAIL_H_
#define _SPRITE2_TRAIL_H_

#include "pre_defined.h"
#include "RenderColor.h"
#include S2_MAT_HEADER

#include <cu/cu_macro.h>

namespace s2
{

class TrailRenderParams
{
public:
	S2_MAT      mat;
	RenderColor ct;
}; // TrailRenderParams

class Trail
{
public:
	void Update(float dt);

	float GetTime() const { return m_time; }

private:
	void Init();

private:
	float m_time;

	CU_SINGLETON_DECLARATION(Trail)

}; // Trail

}

#endif // _SPRITE2_TRAIL_H_