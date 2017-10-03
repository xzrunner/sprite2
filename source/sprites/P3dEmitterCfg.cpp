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
		delete m_impl;
	}
}

void P3dEmitterCfg::SetStartRadius(float radius)
{
	m_impl->start_radius = radius;
}

}