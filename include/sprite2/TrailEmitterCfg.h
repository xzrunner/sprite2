#ifndef _SPRITE2_TRAIL_EMITTER_SYMBOL_H_
#define _SPRITE2_TRAIL_EMITTER_SYMBOL_H_

#include "sprite2/typedef.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <memory>

struct t2d_emitter_cfg;

namespace s2
{

class Symbol;

class TrailEmitterCfg : private cu::Uncopyable
{
public:
	TrailEmitterCfg();
	TrailEmitterCfg(t2d_emitter_cfg* impl);
	virtual ~TrailEmitterCfg();

	const t2d_emitter_cfg* GetImpl() const { return m_impl; }

	void InsertCachedSym(const SymPtr& sym) { m_cached_sym.push_back(sym); }
	void RemoveCachedSym(const Symbol* sym);
	void ClearCachedSym() { m_cached_sym.clear(); }

private:
	t2d_emitter_cfg* m_impl;

	CU_VEC<SymPtr> m_cached_sym;

}; // TrailEmitterCfg

}

#endif // _SPRITE2_TRAIL_EMITTER_SYMBOL_H_
