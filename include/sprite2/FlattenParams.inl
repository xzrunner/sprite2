#ifndef _SPRITE2_FLATTEN_PARAMS_INL_
#define _SPRITE2_FLATTEN_PARAMS_INL_

#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "Utility.h"

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
	: m_mt(params.m_mt)
	, m_spr(NULL)
	, m_actor(NULL)
{
}

inline
FlattenParams& FlattenParams::operator = (const FlattenParams& params)
{
	m_mt = params.m_mt;
	m_spr = NULL;
	m_actor = NULL;
	return *this;
}

inline
void FlattenParams::Push(const Sprite* spr, const Actor* actor)
{
	if (!spr) {
		return;
	}

	m_spr = spr;
	m_actor = actor;

	Utility::PrepareMat(m_mt, spr, actor, m_mt);
}

}

#endif // _SPRITE2_FLATTEN_PARAMS_INL_