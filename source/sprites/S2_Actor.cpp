#include "S2_Actor.h"
#include "S2_Sprite.h"
// #include "FixActorPathVisitor.h"
#include "SprVisitorParams.h"
#include "SprDefault.h"
#include "SprSRT.h"
#include "SprRender.h"

#include "S2_Sprite.h"
#include "S2_Symbol.h"

namespace s2
{

#ifdef S2_RES_LOG
static int COUNT = 0;
#endif // S2_RES_LOG

Actor::Actor()
	: m_spr(NULL)
	, m_parent(NULL)
	, m_geo(ActorDefault::Instance()->Geo())
	, m_render(SprDefault::Instance()->Render())
{
	InitFlags();
}

Actor::Actor(const Sprite* spr, const Actor* parent)
	: m_spr(spr)
	, m_parent(parent)
	, m_geo(ActorDefault::Instance()->Geo())
	, m_render(SprDefault::Instance()->Render())
{
#ifdef S2_RES_LOG
	++COUNT;
	std::cout << "++ actor " << COUNT << "\n";
#endif // S2_RES_LOG

	if (m_spr) {
		m_spr->AddActor(this);
	}

	InitFlags();

	m_aabb.Init(this);
}

Actor::~Actor()
{
#ifdef S2_RES_LOG
	--COUNT;
	std::cout << "-- actor " << COUNT << "\n";
#endif // S2_RES_LOG

	if (m_geo != ActorDefault::Instance()->Geo()) {
		delete m_geo;
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
		m_geo = new ActorGeo;
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
		m_geo = new ActorGeo;
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
		m_geo = new ActorGeo;
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
	if (m_flags & FLAG_VISIBLE == flag) {
		return;
	}
	if (flag) {
		m_flags |= FLAG_VISIBLE;
	} else {
		m_flags &= ~FLAG_VISIBLE;
	}
	if (up_aabb) {
		m_aabb.UpdateParent(this);
		SetFlattenDirtyToRoot();
	}
}

}