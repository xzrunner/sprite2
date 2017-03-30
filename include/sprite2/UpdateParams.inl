#ifndef _SPRITE2_UPDATE_PARAMS_INL_
#define _SPRITE2_UPDATE_PARAMS_INL_

#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "RenderShader.h"
#include "RenderFilter.h"

namespace s2
{

inline
UpdateParams::UpdateParams()
	: m_prev_filter(FM_NULL)
	, m_actor(NULL)
{
}

inline
UpdateParams::UpdateParams(const Actor* actor)
	: m_prev_filter(FM_NULL)
	, m_actor(actor)
{
}

inline
UpdateParams::UpdateParams(const UpdateParams& params)
	: m_prev_mat(params.m_prev_mat)
	, m_prev_filter(FM_NULL)
	, m_actor(params.m_actor)
{
}

inline
UpdateParams& UpdateParams::operator = (const UpdateParams& params)
{
	m_prev_mat = params.m_prev_mat;
	m_actor    = params.m_actor;
	return *this;
}

inline
void UpdateParams::Push(const Sprite* spr)
{
	if (!spr) {
		return;
	}

	m_prev_mat = spr->GetLocalMat() * m_prev_mat;
	if (m_actor) {
		m_prev_mat = m_actor->GetLocalMat() * m_prev_mat;
	}

	const RenderFilter* filter = spr->GetShader().GetFilter();
	if (filter && filter->GetMode() != FM_NULL) {
		m_prev_filter = filter->GetMode();
	}
	if (m_actor) {
		filter = m_actor->GetShader().GetFilter();
		if (filter && filter->GetMode() != FM_NULL) {
			m_prev_filter = filter->GetMode();
		}
	}
}

inline
void UpdateParams::SetPrevMat(const S2_MAT& mat)
{
	m_prev_mat = mat;
}

inline
const S2_MAT& UpdateParams::GetPrevMat() const 
{ 
	return m_prev_mat; 
}

inline
FilterMode UpdateParams::GetPrevFilter() const
{
	return m_prev_filter;
}

inline
void UpdateParams::SetActor(const Actor* actor)
{
	m_actor = actor;
}

inline
const Actor* UpdateParams::GetActor() const
{
	return m_actor;
}

}

#endif // _SPRITE2_UPDATE_PARAMS_INL_