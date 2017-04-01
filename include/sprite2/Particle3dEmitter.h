#ifndef _SPRITE2_PARTICLE3D_EMITTER_H_
#define _SPRITE2_PARTICLE3D_EMITTER_H_

#include "ObjectPool.h"

#include <CU_Uncopyable.h>

struct p3d_emitter;
struct p3d_emitter_cfg;

namespace s2
{

class P3dRenderParams;

class Particle3dEmitter : private cu::Uncopyable
{
public:
	Particle3dEmitter();
	~Particle3dEmitter();

	bool Update(float time);
	void Draw(const P3dRenderParams& rp) const;

	bool IsLoop() const;
	void SetLoop(bool loop);

	bool IsLocal() const;
	void SetLocal(bool local);

	bool IsFinished() const;

	void Start();
	void Stop();

	void CreateEmitter(const p3d_emitter_cfg* cfg);

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
	Particle3dEmitter* GetNext() const { return m_state.next; }
	void SetNext(Particle3dEmitter* next) { m_state.next = next; }

private:
	union
	{
		struct 
		{
			p3d_emitter* et;
			float        mt[6];
			bool         local;
		};

		Particle3dEmitter* next;

	} m_state;

}; // Particle3dEmitter

typedef ObjectPool<Particle3dEmitter> P3dEmitterPool;

}

#endif // _SPRITE2_PARTICLE3D_EMITTER_H_