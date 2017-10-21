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
	m_rect = curr->GetSprRaw()->GetSymbol()->GetBounding(curr->GetSprRaw());

	UpdateTight(curr);
}

bool ActorAABB::Update(const Actor* curr)
{
	if (m_static) {
		return false;
	}

	sm::rect new_rect = Build(curr);
	if (new_rect == m_rect && m_rect.IsValid()) {
		return false;
	}
	m_rect = new_rect;

	UpdateParent(curr);

	return true;
}

bool ActorAABB::Combine(const Actor* curr, const sm::rect& rect)
{
	if (m_static) {
		return false;
	}

	sm::rect new_rect = m_rect;
	new_rect.Combine(rect);
	if (new_rect == m_rect) {
		return false;
	}
	m_rect = new_rect;

	UpdateParent(curr);

	return true;
}

void ActorAABB::UpdateParent(const Actor* curr)
{
	if (m_static) {
		return;
	}

	auto& parent = curr->GetParent();
	if (!parent) {
		return;
	}

	const ActorAABB& p_aabb = parent->GetAABB();
	if (p_aabb.m_static) {
		return;
	}

	if (curr->IsAABBTight()) 
	{
		std::const_pointer_cast<Actor>(parent)->GetAABB().m_rect.MakeEmpty();
		const_cast<ActorAABB&>(p_aabb).Update(parent.get());
		UpdateTight(curr);
	} 
	else 
	{
		sm::rect rect = UpdateTight(curr);
		if (!sm::is_rect_contain_rect(p_aabb.GetRect(), rect)) {
			const_cast<ActorAABB&>(p_aabb).Combine(parent.get(), rect);
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

sm::rect ActorAABB::UpdateTight(const Actor* curr)
{
	if (!m_rect.IsValid()) {
		return m_rect;
	}

	sm::vec2 bounding[4] = {
		sm::vec2(m_rect.xmin, m_rect.ymin),
		sm::vec2(m_rect.xmax, m_rect.ymin),
		sm::vec2(m_rect.xmax, m_rect.ymax),
		sm::vec2(m_rect.xmin, m_rect.ymax),
	};

	sm::rect trans_r;
	S2_MAT mat;
	sm::Matrix2D::Mul(curr->GetSprRaw()->GetLocalMat(), curr->GetLocalMat(), mat);
	for (int i = 0; i < 4; ++i) {
		trans_r.Combine(mat * bounding[i]);
	}

	auto& parent = curr->GetParent();
	if (!parent) {
		return trans_r;
	}

	bool tight = parent && IsRectTight(trans_r, parent->GetAABB().GetRect());
	curr->SetAABBTight(tight);

	return trans_r;
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
	auto spr = curr->GetSprRaw();
	sm::rect r = spr->GetSymbol()->GetBounding(spr, curr, false);
	return r;
}

}