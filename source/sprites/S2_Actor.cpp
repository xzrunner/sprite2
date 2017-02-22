#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "ActorGeo.h"
#include "FixActorPathVisitor.h"
#include "SprVisitorParams.h"

namespace s2
{

static int COUNT = 0;

Actor::Actor(const Sprite* spr, const SprTreePath& path)
	: m_spr(spr)
	, m_path(path)
	, m_geo(NULL)
	, m_proxy(NULL)
{
	++COUNT;

	printf("++ actor %d\n", COUNT);

	if (m_spr) {
		m_spr->AddActor(this);
	}

	InitFlags();
}

Actor::~Actor()
{
	--COUNT;
	if (m_geo) {
		delete m_geo;
	}
	if (m_proxy) {
		m_proxy->RemoveReference();
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

S2_MAT Actor::GetLocalMat() const
{
	if (!m_geo) {
		return S2_MAT();
	}

	S2_MAT mt;
	mt.SetTransformation(m_geo->GetPosition().x, m_geo->GetPosition().y, m_geo->GetAngle(), 
		m_geo->GetScale().x, m_geo->GetScale().y, 0, 0, 0, 0);
	return mt;
}

void Actor::SetProxy(Sprite* proxy)
{
	cu::RefCountObjAssign(m_proxy, proxy);
	if (!proxy) {
		return;
	}

	m_spr->SetHasProxy(true);

	SprTreePath parent_path = m_path;
	parent_path.Pop();
	FixActorPathVisitor visitor(parent_path);
	m_proxy->Traverse(visitor, SprVisitorParams());
}

void Actor::InitFlags()
{
	SetVisible(true);
}

}