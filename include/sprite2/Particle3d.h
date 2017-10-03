#ifndef _SPRITE2_PARTICLE3D_H_
#define _SPRITE2_PARTICLE3D_H_

#include <cu/cu_macro.h>

namespace s2
{

class Particle3d
{
public:
	bool BufferUpdate(float dt);
	void BufferDraw(float x = 0, float y = 0, float scale = 1) const;

	void BufferClear();

	float GetTime() const { return m_time; }

	void Tick();
	void GC();

private:
	void Init();

private:
	float m_time;

	CU_SINGLETON_DECLARATION(Particle3d)

}; // Particle3d

}

#endif // _SPRITE2_PARTICLE3D_H_