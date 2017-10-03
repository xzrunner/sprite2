#ifndef _SPRITE2_PARTICLE2D_H_
#define _SPRITE2_PARTICLE2D_H_

#include <cu/cu_macro.h>

namespace s2
{

class Particle2d
{
public:
	void Update(float dt);
	
	float GetTime() const { return m_time; }

private:
	void Init();

private:
	float m_time;

	CU_SINGLETON_DECLARATION(Particle2d)

}; // Particle2d

}

#endif // _SPRITE2_PARTICLE2D_H_