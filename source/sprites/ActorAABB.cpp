#include "ActorAABB.h"
#include "BoundingBox.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "S2_Symbol.h"

#include <SM_Test.h>

namespace s2
{

ActorAABB::ActorAABB()
	: m_static(false)
{
}

void ActorAABB::Init(const Actor* curr)
{	
	if (m_static) {
		return;
	}

	// not pass actor, in ctor
	m_rect = curr->GetSpr()->GetSymbol()->GetBounding(curr->GetSpr());

	UpdateTight(curr);
}

void ActorAABB::Update(const Actor* curr)
{
	if (m_static) {
		return;
	}

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
	if (m_static) {
		return;
	}

	sm::rect new_rect = m_rect;
	new_rect.Combine(rect);
	if (new_rect == m_rect) {
		return;
	}
	m_rect = new_rect;

	UpdateParent(curr);
	UpdateTight(curr);
}

void ActorAABB::UpdateParent(const Actor* curr)
{
	if (m_static) {
		return;
	}

	const Actor* parent = curr->GetParent();
	if (!parent) {
		return;
	}

	const ActorAABB& p_aabb = parent->GetAABB();
	if (curr->IsAABBTight()) 
	{
		const_cast<Actor*>(parent)->GetAABB().m_rect.MakeEmpty();
		const_cast<ActorAABB&>(p_aabb).Update(parent);
	} 
	else 
	{
		sm::vec2 bounding[4] = {
			sm::vec2(m_rect.xmin, m_rect.ymin),
			sm::vec2(m_rect.xmax, m_rect.ymin),
			sm::vec2(m_rect.xmax, m_rect.ymax),
			sm::vec2(m_rect.xmin, m_rect.ymax),
		};

		sm::rect trans_r;
		sm::mat4 mat = curr->GetSpr()->GetLocalMat() * curr->GetLocalMat();
		for (int i = 0; i < 4; ++i) {
			trans_r.Combine(mat * bounding[i]);
		}
		
		if (!sm::is_rect_contain_rect(p_aabb.GetRect(), trans_r)) {
			const_cast<ActorAABB&>(p_aabb).Combine(parent, trans_r);
		}
	}
}

void ActorAABB::SetRect(const sm::rect& rect) 
{ 
	if (!m_static) {
		m_rect = rect; 
	}
}

void ActorAABB::SetStaticRect(const sm::rect& rect)
{
	m_static = true;
	m_rect = rect;
}

void ActorAABB::UpdateTight(const Actor* curr)
{
	if (m_static) {
		return;
	}

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
	return r;
}

}