#ifndef _SPRITE2_FLATTEN_PARAMS_INL_
#define _SPRITE2_FLATTEN_PARAMS_INL_

#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "sprite2/Utility.h"

namespace s2
{

inline
FlattenParams::FlattenParams()
	: m_spr(NULL)
	, m_actor(NULL)
{
}

inline
FlattenParams::FlattenParams(const FlattenParams& params)
	: m_prev_mt(params.m_prev_mt)
	, m_spr(NULL)
	, m_actor(NULL)
{
}

inline
FlattenParams& FlattenParams::operator = (const FlattenParams& params)
{
	m_prev_mt = params.m_prev_mt;
	m_spr = NULL;
	m_actor = NULL;
	return *this;
}

}

#endif // _SPRITE2_FLATTEN_PARAMS_INL_