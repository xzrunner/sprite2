#ifndef _SPRITE2_TRAIL_EMITTER_SYMBOL_H_
#define _SPRITE2_TRAIL_EMITTER_SYMBOL_H_

#include "s2_typedef.h"

#include <cu/uncopyable.h>

#include <memory>
#include <vector>

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

private:
	t2d_emitter_cfg* m_impl;

	std::vector<SymPtr> m_symbols;

}; // TrailEmitterCfg

}

#endif // _SPRITE2_TRAIL_EMITTER_SYMBOL_H_
