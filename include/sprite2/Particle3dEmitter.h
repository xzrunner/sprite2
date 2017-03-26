#ifndef _SPRITE2_PARTICLE3D_EMITTER_H_
#define _SPRITE2_PARTICLE3D_EMITTER_H_

namespace s2
{

class Particle3dEmitter : public cu::RefCountObj
{
public:
	
	
private:
	p3d_emitter* m_et;

	float m_mt[6];

	bool m_local_mode_draw;

}; // Particle3dEmitter

}

#endif // _SPRITE2_PARTICLE3D_EMITTER_H_