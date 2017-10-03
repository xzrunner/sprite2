#include "AABBHelper.h"
#include "S2_Symbol.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"

#include <SM_Test.h>

namespace s2
{

sm::rect AABBHelper::CalcAABB(const std::vector<SprPtr>& children,
	                          const ActorConstPtr& parent)
{
	sm::rect aabb;

	for (auto& c_spr : children) 
	{
		auto& c_actor = c_spr->QueryActor(parent);
		bool visible = c_actor ? c_actor->IsVisible() : c_spr->IsVisible();
		if (!visible) {
			continue;
		}
		sm::rect c_aabb = c_spr->GetSymbol()->GetBounding(c_spr, c_actor);
		if (!c_aabb.IsValid()) {
			continue;
		}

		S2_MAT mat = c_spr->GetLocalMat();
		if (c_actor) {
			mat = c_actor->GetLocalMat() * mat;
		}
		aabb.Combine(mat * sm::vec2(c_aabb.xmin, c_aabb.ymin));
		aabb.Combine(mat * sm::vec2(c_aabb.xmax, c_aabb.ymin));
		aabb.Combine(mat * sm::vec2(c_aabb.xmax, c_aabb.ymax));
		aabb.Combine(mat * sm::vec2(c_aabb.xmin, c_aabb.ymax));
	}

	UpdateChildrenAABBTight(children, parent, aabb);

	return aabb;
}

void AABBHelper::UpdateChildrenAABBTight(const std::vector<SprPtr>& children,
	                                     const ActorConstPtr& parent,
										 const sm::rect& parent_aabb)
{
	if (!parent) {
		return;
	}
	for (auto& c_spr : children)
	{
		auto& c_actor = c_spr->QueryActor(parent);
		if (!c_actor) {
			continue;
		}

		bool visible = c_actor ? c_actor->IsVisible() : c_spr->IsVisible();
		if (!visible) {
			c_actor->SetAABBTight(false);
			continue;
		}
		sm::rect c_aabb = c_spr->GetSymbol()->GetBounding(c_spr, c_actor);
		if (!c_aabb.IsValid()) {
			c_actor->SetAABBTight(false);
			continue;
		}

		S2_MAT mat = c_spr->GetLocalMat();
		if (c_actor) {
			mat = c_actor->GetLocalMat() * mat;
		}

		if (sm::is_point_on_rect(mat * sm::vec2(c_aabb.xmin, c_aabb.ymin), parent_aabb)) {
			c_actor->SetAABBTight(true);
		} else if (sm::is_point_on_rect(mat * sm::vec2(c_aabb.xmax, c_aabb.ymin), parent_aabb)) {
			c_actor->SetAABBTight(true);
		} else if (sm::is_point_on_rect(mat * sm::vec2(c_aabb.xmax, c_aabb.ymax), parent_aabb)) {
			c_actor->SetAABBTight(true);
		} else if (sm::is_point_on_rect(mat * sm::vec2(c_aabb.xmin, c_aabb.ymax), parent_aabb)) {
			c_actor->SetAABBTight(true);
		} else {
			c_actor->SetAABBTight(true);
		}
	}
}

}