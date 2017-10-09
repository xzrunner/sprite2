#ifndef _SPRITE2_UPDATE_PARAMS_INL_
#define _SPRITE2_UPDATE_PARAMS_INL_

#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "RenderShader.h"
#include "RenderFilter.h"
#include "sprite2/Utility.h"

namespace s2
{

inline
UpdateParams::UpdateParams()
	: m_prev_filter(FM_NULL)
	, m_actor(nullptr)
	, m_force(false)
	, m_next(nullptr)
{
}

inline
UpdateParams::UpdateParams(const Actor* actor)
	: m_prev_filter(FM_NULL)
	, m_actor(actor)
	, m_force(false)
	, m_next(nullptr)
{
}

inline
UpdateParams::UpdateParams(const UpdateParams& params)
	: m_prev_mat(params.m_prev_mat)
	, m_prev_filter(FM_NULL)
	, m_actor(params.m_actor)
	, m_force(params.m_force)
	, m_next(nullptr)
{
}

inline
UpdateParams& UpdateParams::operator = (const UpdateParams& params)
{
	m_prev_mat = params.m_prev_mat;
	m_actor    = params.m_actor;
	m_force    = params.m_force;
	return *this;
}

inline
void UpdateParams::Push(const Sprite* spr)
{
	if (!spr) {
		return;
	}

	S2_MAT mt;
	Utility::PrepareMat(m_prev_mat, spr, m_actor, mt);
	m_prev_mat = mt;

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
void UpdateParams::Init()
{
	m_prev_mat.Identity();
	m_prev_filter = FM_NULL;
	m_actor = nullptr;
	m_force = false;
}

inline
void UpdateParams::Term()
{
}

}

#endif // _SPRITE2_UPDATE_PARAMS_INL_