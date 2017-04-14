#include "ActorAABB.h"
#include "BoundingBox.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "S2_Symbol.h"

#include <SM_Test.h>

namespace s2
{

void ActorAABB::Init(const Actor* curr)
{
	// not pass actor, in ctor
	m_rect = curr->GetSpr()->GetSymbol()->GetBounding(curr->GetSpr());

	UpdateTight(curr);
}

void ActorAABB::Update(const Actor* curr)
{
	sm::rect new_rect = Build(curr);
	if (new_rect == m_rect) {
		return;
	}
	m_rect = new_rect;

	UpdateParent(curr);
	UpdateTight(curr);
}

void ActorAABB::Combine(const Actor* curr, const sm::rect& rect)
{
	sm::rect new_rect = m_rect;
	sm::vec2 bounding[4] = {
		sm::vec2(rect.xmin, rect.ymin),
		sm::vec2(rect.xmax, rect.ymin),
		sm::vec2(rect.xmax, rect.ymax),
		sm::vec2(rect.xmin, rect.ymax),
	};
	const Sprite* spr = curr->GetSpr();
	for (int i = 0; i < 4; ++i) {
		new_rect.Combine(bounding[i]);
	}

	if (new_rect == m_rect) {
		return;
	}
	m_rect = new_rect;

	UpdateParent(curr);
	UpdateTight(curr);
}

void ActorAABB::UpdateParent(const Actor* curr)
{
	const Actor* parent = curr->GetParent();
	if (!parent) {
		return;
	}

	if (curr->IsAABBTight()) {
		const_cast<Actor*>(parent)->GetAABB().Update(parent);
	} else {
		if (!sm::is_rect_contain_rect(parent->GetAABB().GetRect(), m_rect)) {
			const_cast<Actor*>(parent)->GetAABB().Combine(parent, m_rect);
		}
	}
}

void ActorAABB::UpdateTight(const Actor* curr)
{
	const Actor* parent = curr->GetParent();
	bool tight = parent && IsRectTight(m_rect, parent->GetAABB().GetRect());
	curr->SetAABBTight(tight);
}

bool ActorAABB::IsRectTight(const sm::rect& inner, const sm::rect& outer)
{
	return inner.xmin == outer.xmin 
		|| inner.xmax == outer.xmax 
		|| inner.ymin == outer.ymin 
		|| inner.ymax == outer.ymax;
}

sm::rect ActorAABB::Build(const Actor* curr)
{
	const Sprite* spr = curr->GetSpr();
	sm::rect r = spr->GetSymbol()->GetBounding(spr, curr, false);
	sm::vec2 bounding[4] = {
		sm::vec2(r.xmin, r.ymin),
		sm::vec2(r.xmax, r.ymin),
		sm::vec2(r.xmax, r.ymax),
		sm::vec2(r.xmin, r.ymax),
	};

	sm::rect ret;
	for (int i = 0; i < 4; ++i) {
		ret.Combine(bounding[i]);
	}
	return ret;
}

}