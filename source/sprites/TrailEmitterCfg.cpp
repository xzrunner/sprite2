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
	if (m_impl) {
		delete m_impl;
	}
}

}
