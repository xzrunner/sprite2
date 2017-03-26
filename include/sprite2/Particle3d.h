#ifndef _SPRITE2_PARTICLE3D_H_
#define _SPRITE2_PARTICLE3D_H_

#include "RenderParams.h"

#include <CU_Singleton.h>
#include S2_MAT_HEADER

struct p3d_sprite;

namespace s2
{

class P3dRenderParams
{
public:
	S2_MAT mat;
	RenderColor ct;
	p3d_sprite* p3d;

	P3dRenderParams() : p3d(NULL) {}
	P3dRenderParams(P3dRenderParams& p) : mat(p.mat), ct(p.ct), p3d(NULL) {}
	P3dRenderParams& operator = (P3dRenderParams& p) {
		p3d = NULL;
		return *this;
	}
};

class Particle3d
{
public:
	bool BufferUpdate(float dt);
	void BufferDraw(float x = 0, float y = 0, float scale = 1) const;

	void BufferClear();

	float GetTime() const { return m_time; }

private:
	void Init();

private:
	float m_time;

	SINGLETON_DECLARATION(Particle3d)

}; // Particle3d

}

#endif // _SPRITE2_PARTICLE3D_H_