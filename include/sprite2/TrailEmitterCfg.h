#ifndef _SPRITE2_TRAIL_EMITTER_SYMBOL_H_
#define _SPRITE2_TRAIL_EMITTER_SYMBOL_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

struct t2d_emitter_cfg;

namespace s2
{

class TrailEmitterCfg : public cu::RefCountObj, private cu::Uncopyable
{
public:
	TrailEmitterCfg();
	TrailEmitterCfg(t2d_emitter_cfg* impl);
	virtual ~TrailEmitterCfg();

	const t2d_emitter_cfg* GetImpl() const { return m_impl; }

private:
	t2d_emitter_cfg* m_impl;

}; // TrailEmitterCfg

}

#endif // _SPRITE2_TRAIL_EMITTER_SYMBOL_H_
