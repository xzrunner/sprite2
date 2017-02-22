#ifndef _SPRITE2_SPR_GEO_INL_
#define _SPRITE2_SPR_GEO_INL_

namespace s2
{

inline
const sm::vec2& SprGeo::GetPosition() const 
{ 
	return m_state.srt.position; 
}

inline
void SprGeo::SetPosition(const sm::vec2& pos) 
{ 
	m_state.srt.position = pos; 
}

inline
float SprGeo::GetAngle() const 
{ 
	return m_state.srt.angle; 
}

inline
void SprGeo::SetAngle(float angle) 
{ 
	m_state.srt.angle = angle; 
}

inline
const sm::vec2& SprGeo::GetScale() const 
{ 
	return m_state.srt.scale; 
}

inline
void SprGeo::SetScale(const sm::vec2& scale) 
{ 
	m_state.srt.scale = scale; 
}

inline
const sm::vec2& SprGeo::GetShear() const 
{ 
	return m_state.srt.shear; 
}

inline
void SprGeo::SetShear(const sm::vec2& shear) 
{ 
	m_state.srt.shear = shear; 
}

inline
const sm::vec2& SprGeo::GetOffset() const 
{ 
	return m_state.srt.offset; 
}

inline
void SprGeo::SetOffset(const sm::vec2& offset) 
{ 
	m_state.srt.offset = offset; 
}

inline
const sm::vec2& SprGeo::GetCenter() const
{
	return m_state.srt.center;
}

inline
void SprGeo::UpdateCenter()
{
	m_state.srt.UpdateCenter();
}

inline
const SprSRT& SprGeo::GetSRT() const
{
	return m_state.srt;	
}

inline
void SprGeo::SetSRT(const SprSRT& srt)
{
	m_state.srt = srt;
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
	float* dst = m_state.mat.m;
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
	const float* src = m_state.mat.m;
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