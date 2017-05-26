#include "TrailEmitterCfg.h"
#include "S2_Symbol.h"

#include <mt_2d.h>

#include <stddef.h>

namespace s2
{

TrailEmitterCfg::TrailEmitterCfg()
	: m_impl(NULL)
{
}

TrailEmitterCfg::TrailEmitterCfg(t2d_emitter_cfg* impl)
	: m_impl(impl)
{
}

TrailEmitterCfg::~TrailEmitterCfg()
{
	if (!m_impl) {
		return;
	}

	if (m_impl->mode_type == T2D_MODE_IMAGE) {
		static_cast<s2::Symbol*>(m_impl->syms->mode.A.ud)->RemoveReference();
	}

	delete m_impl;
}

}
