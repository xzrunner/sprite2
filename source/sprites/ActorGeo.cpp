#include "ActorGeo.h"

namespace s2
{

void ActorGeo::SetPosition(const sm::vec2& pos) 
{ 
	m_local.position = pos;
	UpdateMatrix();
}

void ActorGeo::SetAngle(float angle) 
{ 
	m_local.angle = angle; 
	UpdateMatrix();
}

void ActorGeo::SetScale(const sm::vec2& scale) 
{ 
	m_local.scale = scale;
	UpdateMatrix();
}

void ActorGeo::UpdateMatrix()
{
	m_local.mat.SetTransformation(GetPosition().x, GetPosition().y, GetAngle(), 
		GetScale().x, GetScale().y, 0, 0, 0, 0);
}

/************************************************************************/
/* class ActorGeo::SRT                                                    */
/************************************************************************/

ActorGeo::SRT::SRT()
	: position(0, 0)
	, angle(0)
	, scale(1, 1)
{
}

}