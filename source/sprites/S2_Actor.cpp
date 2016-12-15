#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "ActorGeo.h"

namespace s2
{

Actor::Actor(const Sprite* spr, const SprTreePath& path)
	: m_spr(spr)
	, m_path(path)
	, m_geo(NULL)
{
	if (m_spr) {
		m_spr->AddActor(this);
	}
}

Actor::~Actor()
{
	if (m_geo) {
		delete m_geo;
	}
}

void Actor::SetPosition(const sm::vec2& pos)
{
	if (m_geo) {
		m_geo->SetPosition(pos);
	} else {
		if (pos != sm::vec2(0, 0)) {
			m_geo = new ActorGeo;
			m_geo->SetPosition(pos);
		}
	}
}

void Actor::SetAngle(float angle)
{
	if (m_geo) {
		m_geo->SetAngle(angle);
	} else {
		if (angle != 0) {
			m_geo = new ActorGeo;
			m_geo->SetAngle(angle);
		}
	}
}

void Actor::SetScale(const sm::vec2& scale)
{
	if (m_geo) {
		m_geo->SetScale(scale);
	} else {
		if (scale != sm::vec2(1, 1)) {
			m_geo = new ActorGeo;
			m_geo->SetScale(scale);
		}
	}
}

sm::mat4 Actor::GetLocalMat() const
{
	if (!m_geo) {
		return sm::mat4();
	}

	sm::mat4 mt;
	mt.SetTransformation(m_geo->GetPosition().x, m_geo->GetPosition().y, m_geo->GetAngle(), 
		m_geo->GetScale().x, m_geo->GetScale().y, 0, 0, 0, 0);
	return mt;
}

}