#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "SymType.h"
#include "ComplexSprite.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "UpdateParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
// #include "FixActorPathVisitor.h"
#include "SprVisitorParams.h"
#include "SymbolVisitor.h"
#include "S2_Actor.h"
#include "AABBHelper.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatDrawCall.h"
#include "sprite2/StatTopNodes.h"
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <SM_Test.h>

#include <map>

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

RenderReturn ComplexSymbol::DrawTree(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	int id = -1;
	if (spr) {
		id = spr->GetSymbol()->GetID();
	}
	StatTopNodes::Checkpoint cp(id, rp.parent_id, rp.level);
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_COMPLEX);
//	StatSymDraw::DrawCostCP cp2(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}

	int action = GetAction(spr, rp.actor.get());

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
		RenderScissor::Instance()->Push(min.x, min.y, max.x-min.x, max.y-min.y, true, false);
	}

	RenderReturn ret = RENDER_OK;

	auto& children = GetActionChildren(action);
	if (rp.IsDisableCulling()) {
		for (auto& child : children) 
		{
			rp_child->actor = child->QueryActor(rp.actor.get());
#ifndef S2_DISABLE_STATISTICS
			rp_child->parent_id = id;
			rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS
			ret |= DrawNode::Draw(child.get(), *rp_child);
		}
	} else {
		for (auto& child : children)
		{
			rp_child->actor = child->QueryActor(rp.actor.get());
#ifndef S2_DISABLE_STATISTICS
			rp_child->parent_id = id;
			rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS
			if (!rp_child->IsDisableCulling() && 
				DrawNode::CullingTestOutside(child.get(), *rp_child)) {
				continue;
			}
			ret |= DrawNode::Draw(child.get(), *rp_child);
		}
	}

	if (scissor) {
		RenderScissor::Instance()->Pop();
	}

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn ComplexSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	return RENDER_SKIP;
}

bool ComplexSymbol::Update(const UpdateParams& up, float time)
{
	bool ret = false;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	for (auto& child : m_children) 
	{
		up_child->SetActor(child->QueryActor(up.GetActor().get()));
		if (child->Update(*up_child)) {
			ret = true;
		}
	}
	UpdateParamsPool::Instance()->Push(up_child); 

	return ret;
}

const std::vector<SprPtr>& ComplexSymbol::GetActionChildren(int action) const
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

int ComplexSymbol::GetActionIdx(const std::string& name) const
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

//bool ComplexSymbol::Change(const SprTreePath& path, const std::string& name, SprPtr dst)
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

//bool ComplexSymbol::Sort(std::vector<SprPtr>& sprs)
//{
//	std::map<int, SprPtr> order_sorted;
//	for (int i = 0, n = sprs.size(); i < n; ++i) {
//		SprPtr obj = sprs[i];
//		for (int j = 0, m = m_children.size(); j < m; ++j) {
//			if (obj == m_children[j].get()) {
//				order_sorted.insert(std::make_pair(j, obj));
//			}
//		}
//	}
//
//	if (order_sorted.size() != sprs.size()) {
//		return false;
//	}
//
//	std::vector<SprPtr> list_dst;
//	list_dst.reserve(sprs.size());
//	std::map<int, SprPtr>::iterator itr = order_sorted.begin();
//	for ( ; itr != order_sorted.end(); ++itr) {
//		list_dst.push_back(itr->second);
//	}
//	sprs = list_dst;
//	return true;
//}

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