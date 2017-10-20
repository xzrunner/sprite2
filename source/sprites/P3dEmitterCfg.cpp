#include "P3dEmitterCfg.h"
#include "S2_Symbol.h"

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
		mm::AllocHelper::Free(m_impl, sz);
	}
}

void P3dEmitterCfg::SetStartRadius(float radius)
{
	m_impl->start_radius = radius;
}

}