#ifndef _SPRITE2_P3D_EMITTER_SYMBOL_H_
#define _SPRITE2_P3D_EMITTER_SYMBOL_H_

#include "sprite2/typedef.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <memory>

struct p3d_emitter_cfg;

namespace s2
{

class Symbol;

class P3dEmitterCfg : private cu::Uncopyable
{
public:
	P3dEmitterCfg();
	P3dEmitterCfg(p3d_emitter_cfg* impl);
	virtual ~P3dEmitterCfg();

	void SetStartRadius(float radius);

	const p3d_emitter_cfg* GetImpl() const { return m_impl; }

	void InsertCachedSym(const SymPtr& sym) { m_cached_sym.push_back(sym); }
	
private:
	p3d_emitter_cfg* m_impl;

	CU_VEC<SymPtr> m_cached_sym;

}; // P3dEmitterCfg

}

#endif // _SPRITE2_P3D_EMITTER_SYMBOL_H_