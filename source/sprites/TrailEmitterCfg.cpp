#include "sprite2/TrailEmitterCfg.h"
#include "sprite2/Symbol.h"

#include <mt_2d.h>

#include <stddef.h>

namespace s2
{

TrailEmitterCfg::TrailEmitterCfg()
	: m_impl(nullptr)
{
}

TrailEmitterCfg::TrailEmitterCfg(t2d_emitter_cfg* impl)
	: m_impl(impl)
{
}

TrailEmitterCfg::~TrailEmitterCfg()
{
	if (m_impl) {
		delete m_impl;
	}
}

void TrailEmitterCfg::RemoveCachedSym(const Symbol* sym)
{
	for (auto itr = m_cached_sym.begin(); itr != m_cached_sym.end(); ++itr) {
		if (itr->get() == sym) {
			m_cached_sym.erase(itr);
			break;
		}
	}
}

}
