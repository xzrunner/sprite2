#ifndef _SPRITE2_ACTOR_GEO_INL_
#define _SPRITE2_ACTOR_GEO_INL_

namespace s2
{

inline
void ActorGeo::Reset()
{
	m_srt.Reset();
	m_mat.Identity();
}

inline
void ActorGeo::SetPosition(const sm::vec2& pos) 
{ 
	m_srt.position = pos;
	UpdateMatrix();
}

inline
void ActorGeo::SetAngle(float angle) 
{ 
	m_srt.angle = angle; 
	UpdateMatrix();
}

inline
void ActorGeo::SetScale(const sm::vec2& scale) 
{ 
	m_srt.scale = scale;
	UpdateMatrix();
}

inline
void ActorGeo::UpdateMatrix()
{
	m_mat.SetTransformation(GetPosition().x, GetPosition().y, GetAngle(), 
		GetScale().x, GetScale().y, 0, 0, 0, 0);
}

/************************************************************************/
/* class ActorGeo::SRT                                                  */
/************************************************************************/

inline
ActorGeo::SRT::SRT()
{
	Reset();
}

inline
void ActorGeo::SRT::Reset()
{
	position.Set(0, 0);
	angle = 0;
	scale.Set(1, 1);
}

}

#endif // _SPRITE2_ACTOR_GEO_INL_