#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "ComplexFlatten.h"
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
#include "Flatten.h"
#include "FlattenParams.h"
#include "AABBHelper.h"
#include "FlattenMgr.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatDrawCall.h"
#include "sprite2/StatTopNodes.h"
#endif // S2_DISABLE_STATISTICS

#include <SM_Test.h>

#include <map>

#include <assert.h>

namespace s2
{

ComplexSymbol::ComplexSymbol()
	: m_scissor(0, 0)
	, m_ft(NULL)
{
}

ComplexSymbol::ComplexSymbol(uint32_t id)
	: Symbol(id)
	, m_scissor(0, 0)
	, m_ft(NULL)
{
}

ComplexSymbol::~ComplexSymbol()
{
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<Sprite>());
	if (m_ft) {
		FlattenMgr::Instance()->Delete(GetID());
		delete m_ft;
	}
}

int ComplexSymbol::Type() const 
{ 
	return SYM_COMPLEX; 
}

void ComplexSymbol::Traverse(const SymbolVisitor& visitor)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		visitor.Visit(m_children[i]);
	}
}

RenderReturn ComplexSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	int id = -1;
	if (spr) {
		id = spr->GetSymbol()->GetID();
	}
	StatTopNodes::Checkpoint cp(id, rp.parent_id, rp.level);
#endif // S2_DISABLE_STATISTICS

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}

	int action = GetAction(spr, rp.actor);

	if (m_ft) {
		RenderReturn ret = m_ft->Draw(*rp_child, action);
		RenderParamsPool::Instance()->Push(rp_child); 
		return ret;
	}

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

	const std::vector<Sprite*>& children = GetActionChildren(action);
	if (rp.IsDisableCulling()) {
		for (int i = 0, n = children.size(); i < n; ++i) 
		{
			const Sprite* child = children[i];
			rp_child->actor = child->QueryActor(rp.actor);
#ifndef S2_DISABLE_STATISTICS
			rp_child->parent_id = id;
			rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS
			ret |= DrawNode::Draw(child, *rp_child);
		}
	} else {
		for (int i = 0, n = children.size(); i < n; ++i) 
		{
			const Sprite* child = children[i];
			rp_child->actor = child->QueryActor(rp.actor);
#ifndef S2_DISABLE_STATISTICS
			rp_child->parent_id = id;
			rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS
			if (DrawNode::CullingTestOutside(child, *rp_child)) {
				continue;
			}
			ret |= DrawNode::Draw(child, *rp_child);
		}
	}

	if (scissor) {
		RenderScissor::Instance()->Pop();
	}

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

bool ComplexSymbol::Update(const UpdateParams& up, float time)
{
	bool ret = false;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		Sprite* child = m_children[i];
		up_child->SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(*up_child)) {
			ret = true;
		}
	}
	UpdateParamsPool::Instance()->Push(up_child); 

	return ret;
}

void ComplexSymbol::Flattening(const FlattenParams& fp, Flatten& ft) const
{
	BuildFlatten(fp.GetActor());

	if (SprNameMap::IsNormalName(fp.GetSpr()->GetName())) 
	{
		ft.AddNode(const_cast<Sprite*>(fp.GetSpr()), const_cast<Actor*>(fp.GetActor()), fp.GetPrevMat());
	} 
	else 
	{
		int action = GetAction(fp.GetSpr(), fp.GetActor());
		const Flatten* curr_ft = m_ft->GetFlatten(action);
		if (curr_ft) {
			S2_MAT mat;
			Utility::PrepareMat(fp.GetPrevMat(), fp.GetSpr(), fp.GetActor(), mat);
			ft.Combine(*curr_ft, mat);
		}
	}
}

const std::vector<Sprite*>& ComplexSymbol::GetActionChildren(int action) const
{
	if (action < 0 || action >= m_actions.size()) {
		return m_children;
	} else {
		if (m_actions[action].sprs.empty()) {
			return m_children;
		} else {
			return m_actions[action].sprs;
		}
	}
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

static void build_flatten(const Actor* actor, const std::vector<Sprite*>& src, Flatten& dst)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		const Sprite* c_spr = src[i];
		const Actor* c_actor = c_spr->QueryActor(actor);
		bool visible = c_actor ? c_actor->IsVisible() : c_spr->IsVisible();
		if (!visible) {
			continue;
		}
		FlattenParams fp;
		fp.SetSpr(c_spr);
		fp.SetActor(c_actor);		
		c_spr->GetSymbol()->Flattening(fp, dst);
	}
}

void ComplexSymbol::BuildFlatten(const Actor* actor) const
{
	// todo: flatten dirty
	// fp.GetActor() && fp.GetActor()->IsFlattenDirty())
	if (m_ft) {
		return;
	}

	std::vector<Flatten> actions;
	if (m_actions.empty())
	{
		actions.resize(1);
		build_flatten(actor, m_children, actions[0]);
	}
	else
	{
		actions.resize(m_actions.size());
		for (int i = 0, n = m_actions.size(); i < n; ++i) {
			const Action& action = m_actions[i];
			build_flatten(actor, action.sprs, actions[i]);
		}
	}

	m_ft = new ComplexFlatten;
	m_ft->SetData(actions);
	FlattenMgr::Instance()->Add(GetID(), m_ft);

	if (actor) {
		actor->SetFlattenDirty(false);
	}
}

bool ComplexSymbol::Add(Sprite* spr, int idx)
{
	spr->AddReference();
	if (m_children.empty() || 
		idx >= m_children.size() ||
		idx < 0) {
		m_children.push_back(spr);
	} else {
		m_children.insert(m_children.begin() + idx, spr);
	}
	m_aabb.MakeEmpty();
	return true;
}

bool ComplexSymbol::Remove(Sprite* spr)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (spr == m_children[i]) {
			spr->RemoveReference();
			m_children.erase(m_children.begin() + i);
			m_aabb.MakeEmpty();
			return true;
		}
	}
	return false;
}

//bool ComplexSymbol::Change(const SprTreePath& path, const std::string& name, Sprite* dst)
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

	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<Sprite>());
	m_children.clear();

	// todo
// 	m_actions.clear();

	m_aabb.MakeEmpty();

	return true;
}

bool ComplexSymbol::ResetOrder(const Sprite* spr, bool up)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] != spr) {
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

bool ComplexSymbol::ResetOrderMost(const Sprite* spr, bool up)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] != spr) {
			continue;
		}
		if (up && i != n - 1) {
			Sprite* tmp = m_children[i];
			for (int j = i + 1; j < n; ++j) {
				m_children[j-1] = m_children[j];
			}
			m_children[n - 1] = tmp;
			return true;
		} else if (!up && i != 0) {
			Sprite* tmp = m_children[i];
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

bool ComplexSymbol::Sort(std::vector<Sprite*>& sprs)
{
	std::map<int, Sprite*> order_sorted;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Sprite* obj = sprs[i];
		for (int j = 0, m = m_children.size(); j < m; ++j) {
			if (obj == m_children[j]) {
				order_sorted.insert(std::make_pair(j, obj));
			}
		}
	}

	if (order_sorted.size() != sprs.size()) {
		return false;
	}

	std::vector<Sprite*> list_dst;
	list_dst.reserve(sprs.size());
	std::map<int, Sprite*>::iterator itr = order_sorted.begin();
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
	const std::vector<Sprite*>& sprs = GetActionChildren(action);
	return AABBHelper::CalcAABB(sprs, actor);
}

int ComplexSymbol::GetAction(const Sprite* spr, const Actor* actor) const
{
	int action = -1;
	if (spr) 
	{
		action = VI_DOWNCASTING<const ComplexSprite*>(spr)->GetAction();
		if (actor) {
			const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
			action = comp_actor->GetAction();
		}
	}
	return action;
}

}