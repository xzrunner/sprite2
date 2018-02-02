#include "sprite2/Actor.h"
#include "sprite2/Sprite.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/CompActorTrans.h"
#include "sprite2/CompDefault.h"
#include "sprite2/CompColorCommon.h"
#include "sprite2/CompColorMap.h"
#include "sprite2/CompCamera.h"
#include "sprite2/CompShader.h"

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
{
	++ALL_ACTOR_COUNT;

	InitFlags();

	auto& caabb = AddComponent<CompActorAABB>();
	caabb.AABB().Init(this);
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

	auto& ctrans = HasComponent<CompActorTrans>() ? GetComponent<CompActorTrans>() : AddComponent<CompActorTrans>();
	ctrans.GetTrans().SetPosition(pos);

	auto& aabb = GetComponent<CompActorAABB>().AABB();
	aabb.SetRect(sm::rect()); // make it empty
	aabb.Update(this);
}

void Actor::SetAngle(float angle)
{
	if (GetAngle() == angle) {
		return;
	}

	auto& ctrans = HasComponent<CompActorTrans>() ? GetComponent<CompActorTrans>() : AddComponent<CompActorTrans>();
	ctrans.GetTrans().SetAngle(angle);

	auto& aabb = GetComponent<CompActorAABB>().AABB();
	aabb.SetRect(sm::rect()); // make it empty
	aabb.Update(this);
}

void Actor::SetScale(const sm::vec2& scale)
{
	if (GetScale() == scale) {
		return;
	}

	auto& ctrans = HasComponent<CompActorTrans>() ? GetComponent<CompActorTrans>() : AddComponent<CompActorTrans>();
	ctrans.GetTrans().SetScale(scale);

	auto& aabb = GetComponent<CompActorAABB>().AABB();
	aabb.SetRect(sm::rect()); // make it empty
	aabb.Update(this);
}

const pt2::RenderColorCommon& Actor::GetColorCommon() const
{
	return HasComponent<CompColorCommon>() ?
		GetComponent<CompColorCommon>().GetColor() : CompDefault::Instance()->Color().GetColor();
}

const pt2::RenderColorMap& Actor::GetColorMap() const
{
	return HasComponent<CompColorMap>() ?
		GetComponent<CompColorMap>().GetColor() : CompDefault::Instance()->ColorMap().GetColor();
}

const pt2::RenderCamera& Actor::GetCamera() const
{
	return HasComponent<CompCamera>() ?
		GetComponent<CompCamera>().GetCamera() : CompDefault::Instance()->Camera().GetCamera();
}

const pt2::RenderShader& Actor::GetShader() const
{
	return HasComponent<CompShader>() ?
		GetComponent<CompShader>().GetShader() : CompDefault::Instance()->Shader().GetShader();
}

void Actor::SetColorCommon(const pt2::RenderColorCommon& col)
{
	if (GetColorCommon() == col) {
		return;
	}

	auto& ccol = HasComponent<CompColorCommon>() ? GetComponent<CompColorCommon>() : AddComponent<CompColorCommon>();
	ccol.SetColor(col);
	SetColorDirty(true);
}

void Actor::SetColorMap(const pt2::RenderColorMap& col)
{
	if (GetColorMap() == col) {
		return;
	}

	auto& ccol = HasComponent<CompColorMap>() ? GetComponent<CompColorMap>() : AddComponent<CompColorMap>();
	ccol.SetColor(col);
	SetColorDirty(true);
}

void Actor::SetCamera(const pt2::RenderCamera& camera)
{
	if (GetCamera() == camera) {
		return;
	}
	auto& ccamera = HasComponent<CompCamera>() ? GetComponent<CompCamera>() : AddComponent<CompCamera>();
	ccamera.SetCamera(camera);
}

void Actor::SetShader(const pt2::RenderShader& shader)
{
	if (GetShader() == shader) {
		return;
	}
	auto& cshader = HasComponent<CompShader>() ? GetComponent<CompShader>() : AddComponent<CompShader>();
	cshader.SetShader(shader);
}

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
		auto& aabb = GetComponent<CompActorAABB>().AABB();
		aabb.UpdateParent(this);
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

const ActorGeoTrans& Actor::GetTransform() const
{
	auto& ctrans = HasComponent<CompActorTrans>() ?
		GetComponent<CompActorTrans>() : CompDefault::Instance()->ActorTrans();
	return ctrans.GetTrans();
}

}