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
#include "Flatten.h"
#include "FlattenParams.h"

#include <SM_Test.h>

#include <map>

#include <assert.h>

namespace s2
{

ComplexSymbol::ComplexSymbol()
	: m_scissor(0, 0)
#ifdef S2_USE_FLATTEN
	, m_ft(NULL)
#endif // S2_USE_FLATTEN
{
}

ComplexSymbol::ComplexSymbol(uint32_t id)
	: Symbol(id)
	, m_scissor(0, 0)
#ifdef S2_USE_FLATTEN
	, m_ft(NULL)
#endif // S2_USE_FLATTEN
{
}

ComplexSymbol::~ComplexSymbol()
{
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<Sprite>());
#ifdef S2_USE_FLATTEN
	if (m_ft) {
		delete m_ft;
	}
#endif // S2_USE_FLATTEN
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

void ComplexSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	RenderParams rp_child(rp);
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

#ifdef S2_USE_FLATTEN
	if (m_ft) {
		m_ft->Draw(rp);
		return;
	}
#endif // S2_USE_FLATTEN

	sm::vec2 scissor_sz = m_scissor.Size();
	bool scissor = scissor_sz.x > 0 && scissor_sz.y > 0;
	if (scissor) 
	{
		sm::vec2 min = rp_child.mt * sm::vec2(m_scissor.xmin, m_scissor.ymin),
			     max = rp_child.mt * sm::vec2(m_scissor.xmax, m_scissor.ymax);
		if (min.x > max.x) {
			std::swap(min.x, max.x);
		}
		if (min.y > max.y) {
			std::swap(min.y, max.y);
		}
		RenderScissor::Instance()->Push(min.x, min.y, max.x-min.x, max.y-min.y, true, false);
	}

	int action = GetAction(spr, rp.actor);
	const std::vector<Sprite*>& sprs = GetActionChildren(action);
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		const Sprite* spr = sprs[i];
		rp_child.actor = spr->QueryActor(rp.actor);
		if (IsChildOutside(spr, rp_child)) {
			continue;
		}
		DrawNode::Draw(spr, rp_child, false);
	}

	if (scissor) {
		RenderScissor::Instance()->Pop();
	}
}

bool ComplexSymbol::Update(const UpdateParams& up, float time)
{
	bool ret = false;
	UpdateParams up_child(up);
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		Sprite* child = m_children[i];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(up_child)) {
			ret = true;
		}
	}
	return ret;
}

void ComplexSymbol::Flattening(const FlattenParams& fp, Flatten& ft) const
{
#ifdef S2_USE_FLATTEN
	if (!m_ft) {
		BuildFlatten();
	}
	ft.Combine(*m_ft, fp.GetMat());
#endif // S2_USE_FLATTEN
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

#ifdef S2_USE_FLATTEN
void ComplexSymbol::BuildFlatten() const
{
	if (m_ft) {
		m_ft->Clear();
	} else {
		m_ft = new Flatten;
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		const Sprite* child = m_children[i];
		FlattenParams fp;
		fp.Push(child);
		child->GetSymbol()->Flattening(fp, *m_ft);		
	}
}
#endif // S2_USE_FLATTEN

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

bool ComplexSymbol::IsChildOutside(const Sprite* spr, const RenderParams& rp) const
{
	RenderScissor* rs = RenderScissor::Instance();
	if (rs->Empty() && !rp.view_region.IsValid()) {
		return false;
	}

	sm::vec2 r_min, r_max;
	sm::rect r = spr->GetSymbol()->GetBounding(spr, rp.actor);
	r_min.Set(r.xmin, r.ymin);
	r_max.Set(r.xmax, r.ymax);
	S2_MAT mat = DrawNode::PrepareMat(rp, spr);
	r_min = mat * r_min;
	r_max = mat * r_max;

	sm::rect sr(r_min, r_max);

	if (!rs->Empty() && rs->IsOutside(sr)) {
		return true;
	}
	if (rp.view_region.IsValid() && !sm::is_rect_intersect_rect(rp.view_region, sr)) {
		return true;
	}
	return false;
}

sm::rect ComplexSymbol::CalcAABB(const Sprite* spr, const Actor* actor) const
{
	sm::vec2 scissor_sz = m_scissor.Size();
	if (scissor_sz.x > 0 && scissor_sz.y > 0) {
		return m_scissor;
	}

	sm::rect aabb;

	int action = GetAction(spr, actor);
	const std::vector<Sprite*>& sprs = GetActionChildren(action);

	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		const Sprite* c_spr = sprs[i];
		const Actor* c_actor = c_spr->QueryActor(actor);
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

	return aabb;
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