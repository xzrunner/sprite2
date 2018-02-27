#include "sprite2/ComplexSymbol.h"
#include "sprite2/ComplexActor.h"
#include "sprite2/SymType.h"
#include "sprite2/ComplexSprite.h"
#include "sprite2/Sprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/DrawNode.h"
// #include "sprite2/FixActorPathVisitor.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/SymbolVisitor.h"
#include "sprite2/Actor.h"
#include "sprite2/AABBHelper.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatDrawCall.h"
#include "sprite2/StatTopNodes.h"
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/RenderScissor.h>
#include <painting2/Blackboard.h>
#include <painting2/Context.h>

#include <cu/cu_stl.h>
#include <SM_Test.h>

#include <assert.h>

namespace s2
{

ComplexSymbol::ComplexSymbol()
	: m_scissor(0, 0)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

ComplexSymbol::ComplexSymbol(uint32_t id)
	: Symbol(id)
	, m_scissor(0, 0)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

ComplexSymbol::~ComplexSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

int ComplexSymbol::Type() const 
{ 
	return SYM_COMPLEX; 
}

void ComplexSymbol::Traverse(const SymbolVisitor& visitor)
{
	for (auto& child : m_children) {
		visitor.Visit(child);
	}
}

pt2::RenderReturn ComplexSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	int id = -1;
	if (spr) {
		id = spr->GetSymbol()->GetID();
	}
	//StatTopNodes::Checkpoint cp(id, rp.parent_id, rp.level);
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_COMPLEX);
//	StatSymDraw::DrawCostCP cp2(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

	int action = GetAction(spr, rp.actor);

	sm::vec2 scissor_sz = m_scissor.Size();
	bool scissor = scissor_sz.x > 0 && scissor_sz.y > 0;
	if (scissor) 
	{
#ifndef S2_DISABLE_STATISTICS
		StatDrawCall::Instance()->AddScissor();
#endif // S2_DISABLE_STATISTICS

		sm::vec2 min = rp_child->mt * sm::vec2(m_scissor.xmin, m_scissor.ymin),
			     max = rp_child->mt * sm::vec2(m_scissor.xmax, m_scissor.ymax);
		if (min.x > max.x) {
			std::swap(min.x, max.x);
		}
		if (min.y > max.y) {
			std::swap(min.y, max.y);
		}
		auto& pt2_ctx = pt2::Blackboard::Instance()->GetContext();
		pt2_ctx.GetScissor().Push(min.x, min.y, max.x-min.x, max.y-min.y, true, false);
	}

	pt2::RenderReturn ret = pt2::RENDER_OK;

	auto& children = GetActionChildren(action);
	if (!children.empty())
	{
		if (rp.IsDisableCulling()) {
			const SprPtr* child_ptr = &children[0];
			for (int i = 0, n = children.size(); i < n; ++i, ++child_ptr)
			{
				rp_child->actor = (*child_ptr)->QueryActor(rp.actor);
	#ifndef S2_DISABLE_STATISTICS
				rp_child->parent_id = id;
				rp_child->level = rp.level + 1;
	#endif // S2_DISABLE_STATISTICS
				ret |= DrawNode::Draw(dlist, (*child_ptr).get(), *rp_child);
			}
		} else {
			const SprPtr* child_ptr = &children[0];
			for (int i = 0, n = children.size(); i < n; ++i, ++child_ptr)
			{
				rp_child->actor = (*child_ptr)->QueryActor(rp.actor);
	#ifndef S2_DISABLE_STATISTICS
				rp_child->parent_id = id;
				rp_child->level = rp.level + 1;
	#endif // S2_DISABLE_STATISTICS
				if (!rp_child->IsDisableCulling() && 
					DrawNode::CullingTestOutside((*child_ptr).get(), *rp_child)) {
					continue;
				}
				ret |= DrawNode::Draw(dlist, (*child_ptr).get(), *rp_child);
			}
		}
	}

	if (scissor) {
		auto& pt2_ctx = pt2::Blackboard::Instance()->GetContext();
		pt2_ctx.GetScissor().Pop();
	}

	return ret;
}

bool ComplexSymbol::Update(const UpdateParams& up, float time)
{
	bool ret = false;

	UpdateParams up_child(up);
	for (auto& child : m_children) 
	{
		up_child.SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(up_child)) {
			ret = true;
		}
	}

	return ret;
}

const CU_VEC<SprPtr>& ComplexSymbol::GetActionChildren(int action) const
{
	if (action < 0 || action >= static_cast<int>(m_actions.size())) {
		return m_children;
	} else {
		if (m_actions[action].sprs.empty()) {
			return m_children;
		} else {
			return m_actions[action].sprs;
		}
	}
}

void ComplexSymbol::SetScissor(const sm::rect& scissor) 
{ 
	m_scissor = scissor; 
	m_aabb = scissor;
}

int ComplexSymbol::GetActionIdx(const CU_STR& name) const
{
	int idx = -1;
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		if (m_actions[i].name == name) {
			idx = i;
			break;
		}
	}
	return idx;
}

bool ComplexSymbol::Add(const SprPtr& spr, int idx)
{
	if (m_children.empty() || 
		idx >= static_cast<int>(m_children.size()) ||
		idx < 0) {
		m_children.push_back(spr);
	} else {
		m_children.insert(m_children.begin() + idx, spr);
	}
	m_aabb.MakeEmpty();
	return true;
}

bool ComplexSymbol::Remove(const Sprite& spr)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		if (&spr == m_children[i].get()) {
			m_children.erase(m_children.begin() + i);
			m_aabb.MakeEmpty();
			return true;
		}
	}
	return false;
}

//bool ComplexSymbol::Change(const SprTreePath& path, const CU_STR& name, SprPtr dst)
//{
//	int idx = -1;
//	for (int i = 0, n = m_children.size(); i < n; ++i) {
//		if (m_children[i]->GetName() == name) {
//			idx = i;
//			break;
//		}
//	}
//
//	if (idx == -1 || m_children[idx] == dst) {
//		return false;
//	}
//
//	if (!dst) {
//		m_children[idx]->RemoveReference();
//		m_children.erase(m_children.begin() + idx);
//		m_size.MakeEmpty();
//		return true;
//	}
//
//	assert(m_children[idx]);
//
//	m_children[idx]->RemoveReference();
//	m_children[idx] = dst;
//	dst->SetName(name);
//	dst->AddReference();
//
//	FixActorPathVisitor visitor(path);
//	dst->Traverse(visitor, SprVisitorParams());
//
//	m_size.MakeEmpty();
//
//	return true;
//}

bool ComplexSymbol::Clear()
{
	if (m_children.empty()) {
		return false;
	}

	m_children.clear();

	// todo
// 	m_actions.clear();

	m_aabb.MakeEmpty();

	return true;
}

bool ComplexSymbol::ResetOrder(const Sprite& spr, bool up)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		if (m_children[i].get() != &spr) {
			continue;
		}
		if (up && i != n - 1) {
			std::swap(m_children[i], m_children[i + 1]);
			return true;
		} else if (!up && i != 0) {
			std::swap(m_children[i], m_children[i - 1]);
			return true;
		}
	}
	return false;
}

bool ComplexSymbol::ResetOrderMost(const Sprite& spr, bool up)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		if (m_children[i].get() != &spr) {
			continue;
		}
		if (up && i != n - 1) {
			auto tmp = m_children[i];
			for (int j = i + 1; j < n; ++j) {
				m_children[j-1] = m_children[j];
			}
			m_children[n - 1] = tmp;
			return true;
		} else if (!up && i != 0) {
			auto tmp = m_children[i];
			for (int j = i - 1; j >= 0; --j) {
				m_children[j+1] = m_children[j];
			}
			m_children[0] = tmp;
			return true;
		}
		return false;
	}
	return false;
}

bool ComplexSymbol::Sort(CU_VEC<SprPtr>& sprs)
{
	CU_MAP<int, SprPtr> order_sorted;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		SprPtr obj = sprs[i];
		for (int j = 0, m = m_children.size(); j < m; ++j) {
			if (obj == m_children[j]) {
				order_sorted.insert(std::make_pair(j, obj));
			}
		}
	}

	if (order_sorted.size() != sprs.size()) {
		return false;
	}

	CU_VEC<SprPtr> list_dst;
	list_dst.reserve(sprs.size());
	CU_MAP<int, SprPtr>::iterator itr = order_sorted.begin();
	for ( ; itr != order_sorted.end(); ++itr) {
		list_dst.push_back(itr->second);
	}
	sprs = list_dst;
	return true;
}

sm::rect ComplexSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (!cache) {
		return CalcAABB(spr, actor);
	}

	if (!m_aabb.IsValid()) {
		m_aabb = CalcAABB(spr, actor);
	}
	return m_aabb;
}

sm::rect ComplexSymbol::CalcAABB(const Sprite* spr, const Actor* actor) const
{
	sm::vec2 scissor_sz = m_scissor.Size();
	if (scissor_sz.x > 0 && scissor_sz.y > 0) {
		return m_scissor;
	}

	int action = GetAction(spr, actor);
	auto& sprs = GetActionChildren(action);
	return AABBHelper::CalcAABB(sprs, actor);
}

int ComplexSymbol::GetAction(const Sprite* spr, const Actor* actor) const
{
	int action = -1;
	if (spr) 
	{
		action = S2_VI_DOWN_CAST<const ComplexSprite*>(spr)->GetAction();
		if (actor) {
			auto comp_actor = static_cast<const ComplexActor*>(actor);
			action = comp_actor->GetAction();
		}
	}
	return action;
}

}