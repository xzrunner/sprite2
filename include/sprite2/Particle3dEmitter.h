#ifndef _SPRITE2_PARTICLE3D_EMITTER_H_
#define _SPRITE2_PARTICLE3D_EMITTER_H_

#include "sprite2/RenderReturn.h"

#include <cu/uncopyable.h>
#include <memmgr/Allocator.h>

#include <memory>

namespace s2
{

class P3dRenderParams;
class P3dEmitterCfg;

class Particle3dEmitter : private cu::Uncopyable
{
public:
	Particle3dEmitter();
	~Particle3dEmitter();

	bool Update(float time);
	RenderReturn Draw(const P3dRenderParams& rp, bool alone) const;
	bool PrepareEmitter();

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

	void CreateEmitter(const std::shared_ptr<const P3dEmitterCfg>& cfg);

	const std::shared_ptr<const P3dEmitterCfg>& GetEmitterCfg() const { return m_cfg; }

private:
	std::shared_ptr<const P3dEmitterCfg> m_cfg;

	int   m_et;
	float m_mt[6];
	bool  m_local;

	bool m_active;
	bool m_loop;

}; // Particle3dEmitter

using Particle3dEmitterPtr = std::unique_ptr<Particle3dEmitter, mm::alloc_deleter<mm::Allocator<Particle3dEmitter>>>;

}

#endif // _SPRITE2_PARTICLE3D_EMITTER_H_