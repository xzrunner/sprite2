#ifndef _SPRITE2_UPDATE_PARAMS_INL_
#define _SPRITE2_UPDATE_PARAMS_INL_

#include "sprite2/Sprite.h"
#include "sprite2/Actor.h"
#include "sprite2/Utility.h"

#include <painting2/RenderShader.h>

namespace s2
{

inline
UpdateParams::UpdateParams()
	: m_prev_filter(pt2::FM_NULL)
	, m_actor(nullptr)
	, m_force(false)
{
}

inline
UpdateParams::UpdateParams(const Actor* actor)
	: m_prev_filter(pt2::FM_NULL)
	, m_actor(actor)
	, m_force(false)
{
}

inline
UpdateParams::UpdateParams(const UpdateParams& params)
	: m_prev_mat(params.m_prev_mat)
	, m_prev_filter(pt2::FM_NULL)
	, m_actor(params.m_actor)
	, m_force(params.m_force)
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
void UpdateParams::Reset()
{
	m_prev_mat.Identity();
	m_prev_filter = pt2::FM_NULL;
	m_actor = nullptr;
	m_force = false;
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

	auto& filter = spr->GetShader().GetFilter();
#ifdef S2_FILTER_FULL
	if (filter && filter->GetMode() != pt2::FM_NULL) {
		m_prev_filter = filter->GetMode();
	}
	if (m_actor) {
		auto& filter = m_actor->GetShader().GetFilter();
		if (filter && filter->GetMode() != pt2::FM_NULL) {
			m_prev_filter = filter->GetMode();
		}
	}
#else
	if (filter != pt2::FM_NULL) {
		m_prev_filter = pt2::FM_NULL;
	}
	if (m_actor) {
		auto& filter = m_actor->GetShader().GetFilter();
		if (filter != pt2::FM_NULL) {
			m_prev_filter = filter;
		}
	}
#endif // S2_FILTER_FULL
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
pt2::FilterMode UpdateParams::GetPrevFilter() const
{
	return m_prev_filter;
}

}

#endif // _SPRITE2_UPDATE_PARAMS_INL_