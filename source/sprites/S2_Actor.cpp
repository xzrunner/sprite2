#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "SprVisitorParams.h"
#include "SprDefault.h"
#include "SprSRT.h"
#include "SprRender.h"

#include "S2_Sprite.h"
#include "S2_Symbol.h"

#include <flatten/Flatten.h>

namespace s2
{

static int ALL_ACTOR_COUNT = 0;

Actor::Actor()
	: m_spr(NULL)
	, m_parent(NULL)
	, m_geo(ActorDefault::Instance()->Geo())
	, m_render(SprDefault::Instance()->Render())
	, m_flatten(nullptr)
{
	++ALL_ACTOR_COUNT;

	InitFlags();
}

Actor::Actor(const Sprite* spr, const Actor* parent)
	: m_spr(spr)
	, m_parent(parent)
	, m_geo(ActorDefault::Instance()->Geo())
	, m_render(SprDefault::Instance()->Render())
	, m_flatten(nullptr)
{
	++ALL_ACTOR_COUNT;

	if (m_spr) {
		m_spr->AddActor(this);
	}

	InitFlags();

	m_aabb.Init(this);
}

Actor::~Actor()
{
	--ALL_ACTOR_COUNT;

	if (m_geo != ActorDefault::Instance()->Geo()) {
		ActorGeoPool::Instance()->Push(m_geo);
	}
	if (m_render != SprDefault::Instance()->Render()) {
		SprRenderPool::Instance()->Push(m_render);
	}
}

void Actor::SetPosition(const sm::vec2& pos)
{
	if (GetPosition() == pos) {
		return;
	}

	if (m_geo == ActorDefault::Instance()->Geo()) {
		m_geo = ActorGeoPool::Instance()->Pop();
	}
	m_geo->SetPosition(pos);

	m_aabb.SetRect(sm::rect()); // make it empty
	m_aabb.Update(this);
}

void Actor::SetAngle(float angle)
{
	if (GetAngle() == angle) {
		return;
	}

	if (m_geo == ActorDefault::Instance()->Geo()) {
		m_geo = ActorGeoPool::Instance()->Pop();
	}
	m_geo->SetAngle(angle);

	m_aabb.SetRect(sm::rect()); // make it empty
	m_aabb.Update(this);
}

void Actor::SetScale(const sm::vec2& scale)
{
	if (GetScale() == scale) {
		return;
	}

	if (m_geo == ActorDefault::Instance()->Geo()) {
		m_geo = ActorGeoPool::Instance()->Pop();
	}
	m_geo->SetScale(scale);

	m_aabb.SetRect(sm::rect()); // make it empty
	m_aabb.Update(this);
}

const RenderColor& Actor::GetColor() const
{
	if (!m_render || (m_render && !m_render->GetColor())) {
		return *SprDefault::Instance()->Render()->GetColor();
	} else {
		return *m_render->GetColor();
	}
}

const RenderShader& Actor::GetShader() const
{
	if (!m_render || (m_render && !m_render->GetShader())) {
		return *SprDefault::Instance()->Render()->GetShader();
	} else {
		return *m_render->GetShader();
	}
}

const RenderCamera& Actor::GetCamera() const
{
	if (!m_render || (m_render && !m_render->GetCamera())) {
		return *SprDefault::Instance()->Render()->GetCamera();
	} else {
		return *m_render->GetCamera();
	}
}

void Actor::SetColor(const RenderColor& color)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetColor(color);

	SetColorDirty(true);
}

void Actor::SetShader(const RenderShader& shader)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetShader(shader);
}

void Actor::SetCamera(const RenderCamera& camera)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetCamera(camera);
}

void Actor::SetFlattenDirtyToRoot() const
{
	const Actor* actor = this;
	while (actor) {
		actor->SetFlattenDirty(true);
		actor = actor->GetParent();
	}
}

void Actor::InitFlags()
{
	m_flags = 0;

	SetVisible(true);
#ifdef S2_SPR_DEFAULT_EDITABLE
	SetEditable(S2_SPR_DEFAULT_EDITABLE);
#else
	SetEditable(true);
#endif // S2_SPR_DEFAULT_EDITABLE
}

void Actor::SetVisible(bool flag, bool up_aabb) const
{
	if ((m_flags & FLAG_VISIBLE) == flag) {
		return;
	}
	if (flag) {
		m_flags |= FLAG_VISIBLE;
	} else {
		m_flags &= ~FLAG_VISIBLE;
	}
	if (up_aabb) {
		m_aabb.UpdateParent(this);
//		SetFlattenDirtyToRoot();
	}
}

int Actor::GetAllActorCount()
{
	return ALL_ACTOR_COUNT;
}

}