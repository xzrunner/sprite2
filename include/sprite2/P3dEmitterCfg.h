#ifndef _SPRITE2_P3D_EMITTER_SYMBOL_H_
#define _SPRITE2_P3D_EMITTER_SYMBOL_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

struct p3d_emitter_cfg;

namespace s2
{

class P3dEmitterCfg : public cu::RefCountObj, private cu::Uncopyable
{
public:
	P3dEmitterCfg();
	P3dEmitterCfg(p3d_emitter_cfg* impl);
	virtual ~P3dEmitterCfg();

	void SetStartRadius(float radius);

	const p3d_emitter_cfg* GetImpl() const { return m_impl; }
	
private:
	p3d_emitter_cfg* m_impl;

}; // P3dEmitterCfg

}

#endif // _SPRITE2_P3D_EMITTER_SYMBOL_H_