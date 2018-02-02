#pragma once

namespace s2
{

inline
void ActorGeoTrans::Reset()
{
	m_srt.Reset();
	m_mat.Identity();
}

inline
void ActorGeoTrans::SetPosition(const sm::vec2& pos) 
{ 
	m_srt.position = pos;
	UpdateMatrix();
}

inline
void ActorGeoTrans::SetAngle(float angle) 
{ 
	m_srt.angle = angle; 
	UpdateMatrix();
}

inline
void ActorGeoTrans::SetScale(const sm::vec2& scale) 
{ 
	m_srt.scale = scale;
	UpdateMatrix();
}

inline
void ActorGeoTrans::UpdateMatrix()
{
	m_mat.SetTransformation(GetPosition().x, GetPosition().y, GetAngle(), 
		GetScale().x, GetScale().y, 0, 0, 0, 0);
}

/************************************************************************/
/* class ActorGeoTrans::SRT                                                  */
/************************************************************************/

inline
ActorGeoTrans::SRT::SRT()
{
	Reset();
}

inline
void ActorGeoTrans::SRT::Reset()
{
	position.Set(0, 0);
	angle = 0;
	scale.Set(1, 1);
}

}