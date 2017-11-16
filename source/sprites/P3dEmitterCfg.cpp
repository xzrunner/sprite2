#include "sprite2/P3dEmitterCfg.h"
#include "sprite2/Symbol.h"

#include <ps_3d.h>

#include <stddef.h>

namespace s2
{

P3dEmitterCfg::P3dEmitterCfg()
	: m_impl(nullptr)
{
}

P3dEmitterCfg::P3dEmitterCfg(p3d_emitter_cfg* impl)
	: m_impl(impl)
{
}

P3dEmitterCfg::~P3dEmitterCfg()
{
	if (m_impl) {
		int sz = SIZEOF_P3D_EMITTER_CFG + SIZEOF_P3D_SYMBOL * m_impl->sym_count;
#ifdef USE_MM_ALLOCATOR
		mm::AllocHelper::Free(m_impl, sz);
#else
		delete reinterpret_cast<uint8_t*>(m_impl);
#endif // USE_MM_ALLOCATOR
	}
}

void P3dEmitterCfg::SetStartRadius(float radius)
{
	m_impl->start_radius = radius;
}

void P3dEmitterCfg::RemoveCachedSym(const Symbol* sym)
{
	for (auto itr = m_cached_sym.begin(); itr != m_cached_sym.end(); ++itr) {
		if (itr->get() == sym) {
			m_cached_sym.erase(itr);
			break;
		}
	}
}

}