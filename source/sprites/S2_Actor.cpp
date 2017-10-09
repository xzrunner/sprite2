#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "SprVisitorParams.h"
#include "SprDefault.h"
#include "SprSRT.h"
#include "SprRender.h"

#include "S2_Sprite.h"
#include "S2_Symbol.h"

#ifndef S2_DISABLE_FLATTEN
#include <flatten/FTList.h>
#endif // S2_DISABLE_FLATTEN
#include <cooking/DisplayList.h>

namespace s2
{

static int ALL_ACTOR_COUNT = 0;

Actor::Actor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: m_spr(std::const_pointer_cast<Sprite>(spr).get())
	, m_geo(ActorDefault::Instance()->Geo())
	, m_render(SprDefault::Instance()->Render())
{
	m_spr_ptr = spr;

	m_parent = parent;

	++ALL_ACTOR_COUNT;

	InitFlags();
}

//Actor::Actor()
//	: m_spr(nullptr)
//	, m_geo(ActorDefault::Instance()->Geo())
//	, m_render(SprDefault::Instance()->Render())
//{
//	++ALL_ACTOR_COUNT;
//
//	InitFlags();
//}

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

void Actor::Init()
{
	m_aabb.Init(shared_from_this());
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
	m_aabb.Update(shared_from_this());
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
	m_aabb.Update(shared_from_this());
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
	m_aabb.Update(shared_from_this());
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

void Actor::SetFlattenDirty()
{
	const Actor* curr = this;
	while (curr)
	{
		if (curr->m_flatten.list) {
			curr->m_flatten.list->SetDirty();
			break;
		}

		auto p = curr->m_parent.lock();
		if (p) {
			curr = p.get();
		} else {
			break;
		}
	}
}

void Actor::BuildFlatten()
{
	const Actor* curr = this;
	while (curr) 
	{
		if (curr->m_flatten.list) {
			if (!curr->m_flatten.list->IsDirty()) {
				printf("++++++ should be dirty %d\n", GetSpr()->GetSymbol()->GetID());
			} else {
				curr->m_flatten.list->Build(curr->m_dlist);
			}
			break;
		}

		auto p = curr->m_parent.lock();
		if (p) {
			curr = p.get();
		} else {
			break;
		}
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
		m_aabb.UpdateParent(shared_from_this());
	}
}

void Actor::BuildDisplayList()
{
	m_dlist = std::make_shared<cooking::DisplayList>();
}

int Actor::GetAllActorCount()
{
	return ALL_ACTOR_COUNT;
}

}