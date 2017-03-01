#ifndef _SPRITE2_SPR_GEO_INL_
#define _SPRITE2_SPR_GEO_INL_

#include <SM_Calc.h>

namespace s2
{

inline
sm::vec2 SprGeo::GetPosition() const 
{ 
	return sm::vec2(m_state.srt[SprSRT::IDX_POS_X], m_state.srt[SprSRT::IDX_POS_Y]);
}

inline
void SprGeo::SetPosition(const sm::vec2& pos) 
{ 
	m_state.srt[SprSRT::IDX_POS_X] = pos.x;
	m_state.srt[SprSRT::IDX_POS_Y] = pos.y;
}

inline
float SprGeo::GetAngle() const 
{ 
	return m_state.srt[SprSRT::IDX_ANGLE]; 
}

inline
void SprGeo::SetAngle(float angle) 
{ 
	m_state.srt[SprSRT::IDX_ANGLE] = angle; 
}

inline
sm::vec2 SprGeo::GetScale() const 
{
	return sm::vec2(m_state.srt[SprSRT::IDX_SCALE_X], m_state.srt[SprSRT::IDX_SCALE_Y]);
}

inline
void SprGeo::SetScale(const sm::vec2& scale) 
{ 
	m_state.srt[SprSRT::IDX_SCALE_X] = scale.x;
	m_state.srt[SprSRT::IDX_SCALE_Y] = scale.y;
}

inline
sm::vec2 SprGeo::GetShear() const 
{ 
	return sm::vec2(m_state.srt[SprSRT::IDX_SHEAR_X], m_state.srt[SprSRT::IDX_SHEAR_Y]);
}

inline
void SprGeo::SetShear(const sm::vec2& shear) 
{ 
	m_state.srt[SprSRT::IDX_SHEAR_X] = shear.x;
	m_state.srt[SprSRT::IDX_SHEAR_Y] = shear.y;
}

inline
sm::vec2 SprGeo::GetOffset() const 
{ 
	return sm::vec2(m_state.srt[SprSRT::IDX_OFFSET_X], m_state.srt[SprSRT::IDX_OFFSET_Y]);
}

inline
void SprGeo::SetOffset(const sm::vec2& offset) 
{ 
	m_state.srt[SprSRT::IDX_OFFSET_X] = offset.x;
	m_state.srt[SprSRT::IDX_OFFSET_Y] = offset.y;
}

inline
sm::vec2 SprGeo::GetCenter() const
{
	return sm::vec2(m_state.srt[SprSRT::IDX_CENTER_X], m_state.srt[SprSRT::IDX_CENTER_Y]);
}

inline
void SprGeo::UpdateCenter()
{
	sm::vec2 position(m_state.srt[SprSRT::IDX_POS_X], m_state.srt[SprSRT::IDX_POS_Y]);
	sm::vec2 offset(m_state.srt[SprSRT::IDX_OFFSET_X], m_state.srt[SprSRT::IDX_OFFSET_Y]);
	float angle = m_state.srt[SprSRT::IDX_ANGLE];
	sm::vec2 center = position + sm::rotate_vector(-offset, angle) + offset;
	m_state.srt[SprSRT::IDX_CENTER_X] = center.x;
	m_state.srt[SprSRT::IDX_CENTER_Y] = center.y;
}

inline
void SprGeo::GetSRT(SprSRT& srt) const
{
	memcpy(srt.srt, m_state.srt, sizeof(srt));
}

inline
void SprGeo::SetSRT(const SprSRT& srt)
{
	memcpy(m_state.srt, srt.srt, sizeof(srt));
}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE

inline
void SprGeo::SetMatrix(const S2_MAT& mat)
{
	m_state.mat = mat;
}

inline
const S2_MAT& SprGeo::GetMatrix() const
{
	return m_state.mat;
}

#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY

inline
void SprGeo::SetMatrix(const S2_MAT& mat)
{
	float* dst = m_state.mat;
#ifdef S2_MATRIX_FIX
	const int* src = mat.x;
	dst[0] = src[0] * sm::MatrixFix::SCALE_INV;
	dst[1] = src[1] * sm::MatrixFix::SCALE_INV;
	dst[2] = src[2] * sm::MatrixFix::SCALE_INV;
	dst[3] = src[3] * sm::MatrixFix::SCALE_INV;
	dst[4] = src[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
	dst[5] = src[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
	const float* src = mat.x;
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[4];
	dst[3] = src[5];
	dst[4] = src[12];
	dst[5] = src[13];
#endif // S2_MATRIX_FIX
}

inline
S2_MAT SprGeo::GetMatrix() const
{
	S2_MAT mat;
	const float* src = m_state.mat;
#ifdef S2_MATRIX_FIX
	int* dst = mat.x;
	dst[0] = src[0] * sm::MatrixFix::SCALE;
	dst[1] = src[1] * sm::MatrixFix::SCALE;
	dst[2] = src[2] * sm::MatrixFix::SCALE;
	dst[3] = src[3] * sm::MatrixFix::SCALE;
	dst[4] = src[4] * sm::MatrixFix::TRANSLATE_SCALE;
	dst[5] = src[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
	float* dst = mat.x;
	dst[0] = src[0];
	dst[1] = src[1];
	dst[4] = src[2];
	dst[5] = src[3];
	dst[12]= src[4];
	dst[13]= src[5];
#endif // S2_MATRIX_FIX
	return mat;
}

#endif // S2_SPR_CACHE_LOCAL_MAT_COPY

}

#endif // _SPRITE2_SPR_GEO_INL_