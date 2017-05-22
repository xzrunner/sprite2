#ifndef _SPRITE2_SPR_GEO_INL_
#define _SPRITE2_SPR_GEO_INL_

#include <SM_Calc.h>

namespace s2
{

inline
const sm::vec2& SprGeo::GetPosition() const 
{ 
	return m_srt.position;
}

inline
void SprGeo::SetPosition(const sm::vec2& pos) 
{
	if (pos != m_srt.position) {
		m_srt.position = pos;
		UpdateCenter();
		UpdateMatrix();
	}
}

inline
float SprGeo::GetAngle() const 
{
	return m_srt.angle;
}

inline
void SprGeo::SetAngle(float angle) 
{
	if (angle != m_srt.angle) {
		m_srt.angle = angle;
		UpdateCenter();
		UpdateMatrix();
	}
}

inline
const sm::vec2& SprGeo::GetScale() const 
{
	return m_srt.scale;
}

inline
void SprGeo::SetScale(const sm::vec2& scale) 
{ 
	if (scale != m_srt.scale) {
		m_srt.scale = scale;
		UpdateMatrix();
	}
}

inline
const sm::vec2& SprGeo::GetShear() const 
{
	return m_srt.shear;
}

inline
void SprGeo::SetShear(const sm::vec2& shear) 
{
	if (shear != m_srt.shear) {
		m_srt.shear = shear;
		UpdateMatrix();
	}
}

inline
const sm::vec2& SprGeo::GetOffset() const 
{ 
	return m_srt.offset;
}

inline
void SprGeo::SetOffset(const sm::vec2& offset) 
{ 
	if (offset != m_srt.offset) {
		m_srt.offset = offset;
		UpdateCenter();
		UpdateMatrix();
	}
}

inline
const sm::vec2& SprGeo::GetCenter() const
{
	return m_srt.center;
}

inline
void SprGeo::GetSRT(SprSRT& srt) const
{
	srt.srt[SprSRT::IDX_POS_X] = m_srt.position.x;
	srt.srt[SprSRT::IDX_POS_Y] = m_srt.position.y;

	srt.srt[SprSRT::IDX_ANGLE] = m_srt.angle;

	srt.srt[SprSRT::IDX_SCALE_X] = m_srt.scale.x;
	srt.srt[SprSRT::IDX_SCALE_Y] = m_srt.scale.y;

	srt.srt[SprSRT::IDX_SHEAR_X] = m_srt.shear.x;
	srt.srt[SprSRT::IDX_SHEAR_Y] = m_srt.shear.y;

	srt.srt[SprSRT::IDX_OFFSET_X] = m_srt.offset.x;
	srt.srt[SprSRT::IDX_OFFSET_Y] = m_srt.offset.y;

	srt.srt[SprSRT::IDX_CENTER_X] = m_srt.center.x;
	srt.srt[SprSRT::IDX_CENTER_Y] = m_srt.center.y;
}

inline
void SprGeo::SetSRT(const SprSRT& srt)
{
	m_srt.position.x = srt.srt[SprSRT::IDX_POS_X];
	m_srt.position.y = srt.srt[SprSRT::IDX_POS_Y];

	m_srt.angle = srt.srt[SprSRT::IDX_ANGLE];
	
	m_srt.scale.x = srt.srt[SprSRT::IDX_SCALE_X];
	m_srt.scale.y = srt.srt[SprSRT::IDX_SCALE_Y];

	m_srt.shear.x = srt.srt[SprSRT::IDX_SHEAR_X];
	m_srt.shear.y = srt.srt[SprSRT::IDX_SHEAR_Y];

	m_srt.offset.x = srt.srt[SprSRT::IDX_OFFSET_X];
	m_srt.offset.y = srt.srt[SprSRT::IDX_OFFSET_Y];

	m_srt.center.x = srt.srt[SprSRT::IDX_CENTER_X];
	m_srt.center.y = srt.srt[SprSRT::IDX_CENTER_Y];

	UpdateMatrix();
}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE

inline
void SprGeo::SetMatrix(const S2_MAT& mat)
{
	memcpy(m_state.mat, mat.x, sizeof(m_state.mat));
}

inline
S2_MAT SprGeo::GetMatrix() const
{
	S2_MAT mat;
	memcpy(mat.x, m_state.mat, sizeof(m_state.mat));
	return mat;
}

#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

inline
void SprGeo::UpdateCenter()
{
	m_srt.center = m_srt.position + sm::rotate_vector(-m_srt.offset, m_srt.angle) + m_srt.offset;
}

inline
void SprGeo::UpdateMatrix()
{
	m_mat.SetTransformation(m_srt.center.x, m_srt.center.y, m_srt.angle, m_srt.scale.x, 
		m_srt.scale.y, 0, 0, m_srt.shear.x, m_srt.shear.y);
}

/************************************************************************/
/* class SprGeo::SRT                                                    */
/************************************************************************/

inline
SprGeo::SRT::SRT()
	: position(0, 0)
	, angle(0)
	, scale(1, 1)
	, shear(0, 0)
	, offset(0, 0)
	, center(0, 0)
{
}

}

#endif // _SPRITE2_SPR_GEO_INL_