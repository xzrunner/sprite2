#include "sprite2/Actor.h"
#include "sprite2/Sprite.h"
#include "sprite2/SprVisitorParams.h"

#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"

#ifndef S2_DISABLE_FLATTEN
#include <flatten/FTList.h>
#endif // S2_DISABLE_FLATTEN
#include <cooking/DisplayList.h>

namespace s2
{

static int ALL_ACTOR_COUNT = 0;

Actor::Actor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: m_spr(spr)
	, m_parent(parent)
	, m_geo(ActorDefault::Instance()->Geo(), geo_deleter)
	// todo zz
//	, m_render(SprDefault::Instance()->Render(), render_deleter)
{
	++ALL_ACTOR_COUNT;

	InitFlags();

	m_aabb.Init(this);
}

Actor::~Actor()
{
	--ALL_ACTOR_COUNT;
}

void Actor::SetPosition(const sm::vec2& pos)
{
	if (GetPosition() == pos) {
		return;
	}

	if (m_geo.get() == ActorDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<ActorGeoTrans*>(mm::AllocHelper::New<ActorGeoTrans>()));
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

	if (m_geo.get() == ActorDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<ActorGeoTrans*>(mm::AllocHelper::New<ActorGeoTrans>()));
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

	if (m_geo.get() == ActorDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<ActorGeoTrans*>(mm::AllocHelper::New<ActorGeoTrans>()));
	}
	m_geo->SetScale(scale);

	m_aabb.SetRect(sm::rect()); // make it empty
	m_aabb.Update(this);
}

// todo zz
//void Actor::SetColor(const RenderColor& color)
//{
//	if (m_render.get() == SprDefault::Instance()->Render() || !m_render) {
//		m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
//	}
//	m_render->SetColor(color);
//
//	SetColorDirty(true);
//}

// todo zz
//void Actor::SetShader(const RenderShader& shader)
//{
//	if (m_render.get() == SprDefault::Instance()->Render() || !m_render) {
//		m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
//	}
//	m_render->SetShader(shader);
//}

// todo zz
//void Actor::SetCamera(const pt2::RenderCamera& camera)
//{
//	if (m_render.get() == SprDefault::Instance()->Render() || !m_render) {
//		m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
//	}
//	m_render->SetCamera(camera);
//}

#ifndef S2_DISABLE_FLATTEN

void Actor::SetFlattenDirty()
{
	const Actor* curr = this;
	while (curr)
	{
		if (curr->m_flatten.list) {
			curr->m_flatten.list->SetDirty();
			break;
		}

		auto p = curr->m_parent.smart_ptr.lock();
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
				printf("++++++ should be dirty %d\n", GetSprRaw()->GetSymbol()->GetID());
			} else {
#ifndef S2_DISABLE_DEFERRED
				curr->m_flatten.list->Build(curr->m_dlist);
#else
				curr->m_flatten.list->Build(nullptr);
#endif // S2_DISABLE_DEFERRED
			}
			break;
		}

		auto p = curr->m_parent.smart_ptr.lock();
		if (p) {
			curr = p.get();
		} else {
			break;
		}
	}
}

#endif // S2_DISABLE_FLATTEN

void Actor::InitFlags()
{
	m_flags = 0;

	SetVisible(true);
	SetEditable(S2_SPR_DEFAULT_EDITABLE);
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
	}
}

#ifndef S2_DISABLE_DEFERRED

void Actor::BuildDisplayList()
{
	m_dlist = CU_MAKE_SHARED<cooking::DisplayList>();
}

#endif // S2_DISABLE_DEFERRED

int Actor::GetAllActorCount()
{
	return ALL_ACTOR_COUNT;
}

}