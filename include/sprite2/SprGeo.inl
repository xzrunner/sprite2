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
void SprGeo::SetCenter(const sm::vec2& center)
{
	m_state.srt.center = center;
}

#ifdef S2_SPR_CACHE_LOCAL_MAT

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

#endif // S2_SPR_CACHE_LOCAL_MAT

}

#endif // _SPRITE2_SPR_GEO_INL_