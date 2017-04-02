#ifndef _SPRITE2_PARTICLE3D_EMITTER_H_
#define _SPRITE2_PARTICLE3D_EMITTER_H_

#include "ObjectPool.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

struct p3d_emitter;

namespace s2
{

class P3dRenderParams;
class P3dEmitterCfg;

class Particle3dEmitter : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Particle3dEmitter();
	~Particle3dEmitter();

	virtual void RemoveReference() const;

	bool Update(float time);
	void Draw(const P3dRenderParams& rp, bool alone) const;

	bool IsLoop() const;
	void SetLoop(bool loop);

	bool IsLocal() const;
	void SetLocal(bool local);

	bool IsFinished() const;

	void ResetTime();

	void Start();
	void Stop();

	void Pause();
	void Resume();

	void Clear();

	float GetTime() const;

	void SetMat(float* mat);

	void CreateEmitter(const P3dEmitterCfg* cfg);

	const P3dEmitterCfg* GetEmitterCfg() const { return m_state.cfg; }

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
			const P3dEmitterCfg* cfg;

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