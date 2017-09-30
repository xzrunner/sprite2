#include "TrailEmitterCfg.h"
#include "S2_Symbol.h"

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
	if (!m_impl) {
		return;
	}

	if (m_impl->mode_type == T2D_MODE_IMAGE) {
		for (int i = 0; i < m_impl->sym_count; ++i) {
			void* ud = m_impl->syms[i].mode.A.ud;
			if (!ud) {
				continue;
			}
			static_cast<s2::Symbol*>(ud)->RemoveReference();
		}
	}

	delete m_impl;
}

}
