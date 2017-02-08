#include "ComplexSymbol.h"
#include "SymType.h"
#include "ComplexSprite.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "FixActorPathVisitor.h"
#include "SprVisitorParams.h"
#include "SymVisitor.h"
#include "S2_Actor.h"

#include <SM_Test.h>

#include <map>

#include <assert.h>

namespace s2
{

ComplexSymbol::ComplexSymbol()
	: m_scissor(0, 0)
{
}

ComplexSymbol::ComplexSymbol(uint32_t id)
	: Symbol(id)
	, m_scissor(0, 0)
{
}

ComplexSymbol::~ComplexSymbol()
{
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<Sprite>());
}

int ComplexSymbol::Type() const 
{ 
	return SYM_COMPLEX; 
}

void ComplexSymbol::Traverse(const SymVisitor& visitor)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		visitor.Visit(m_children[i]);
	}
}

void ComplexSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	RenderParams p = DrawNode::Prepare(params, spr);

	sm::vec2 scissor_sz = m_scissor.Size();
	bool scissor = scissor_sz.x > 0 && scissor_sz.y > 0;
	if (scissor) 
	{
		sm::vec2 min = p.mt * sm::vec2(m_scissor.xmin, m_scissor.ymin),
			     max = p.mt * sm::vec2(m_scissor.xmax, m_scissor.ymax);
		if (min.x > max.x) {
			std::swap(min.x, max.x);
		}
		if (min.y > max.y) {
			std::swap(min.y, max.y);
		}
		RenderScissor::Instance()->Push(min.x, min.y, max.x-min.x, max.y-min.y);
	}

	int action = -1;
	if (spr) {
		action = VI_DOWNCASTING<const ComplexSprite*>(spr)->GetAction();
	}
	const std::vector<Sprite*>& sprs = GetActionChildren(action);
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		const Sprite* spr = sprs[i];

		if (spr->IsHasProxy()) 
		{
			SprTreePath path = p.path;
			path.Push(spr->GetID());
			const Actor* actor = spr->QueryActor(path);
			if (actor) {
				const Sprite* proxy = actor->GetProxy();
				if (proxy) {
					spr = proxy;
				}
			}
		}

		if (IsChildOutside(spr, p)) {
			continue;
		}
		DrawNode::Draw(spr, p, false);
	}

	if (scissor) {
		RenderScissor::Instance()->Pop();
	}
}

bool ComplexSymbol::Update(const RenderParams& params, float time)
{
	bool ret = false;
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i]->Update(params)) {
			ret = true;
		}
	}
	return ret;
}

sm::rect ComplexSymbol::GetBounding(const Sprite* spr) const
{
 	if (m_size.IsValid()) {
 		return m_size;
 	}

	sm::vec2 scissor_sz = m_scissor.Size();
	if (scissor_sz.x > 0 && scissor_sz.y > 0) {
		m_size = m_scissor;
		return m_size;
	}

	m_size.MakeEmpty();
	int action = -1;
	if (spr) {
		action = VI_DOWNCASTING<const ComplexSprite*>(spr)->GetAction();
	}
	const std::vector<Sprite*>& sprs = GetActionChildren(action);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->GetBounding()->CombineTo(m_size);
	}

	return m_size;
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
	m_size.MakeEmpty();
	return true;
}

bool ComplexSymbol::Remove(Sprite* spr)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (spr == m_children[i]) {
			spr->RemoveReference();
			m_children.erase(m_children.begin() + i);
			m_size.MakeEmpty();
			return true;
		}
	}
	return false;
}

bool ComplexSymbol::Change(const SprTreePath& path, const std::string& name, Sprite* dst)
{
	int idx = -1;
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i]->GetName() == name) {
			idx = i;
			break;
		}
	}

	if (idx == -1 || m_children[idx] == dst) {
		return false;
	}

	if (!dst) {
		m_children[idx]->RemoveReference();
		m_children.erase(m_children.begin() + idx);
		m_size.MakeEmpty();
		return true;
	}

	assert(m_children[idx]);

	m_children[idx]->RemoveReference();
	m_children[idx] = dst;
	dst->SetName(name);
	dst->AddReference();

	FixActorPathVisitor visitor(path);
	dst->Traverse(visitor, SprVisitorParams());

	m_size.MakeEmpty();

	return true;
}

bool ComplexSymbol::Clear()
{
	if (m_children.empty()) {
		return false;
	}

	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<Sprite>());
	m_children.clear();

	// todo
// 	m_actions.clear();

	m_size.MakeEmpty();

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

bool ComplexSymbol::IsChildOutside(const Sprite* spr, const RenderParams& params) const
{
	RenderScissor* rs = RenderScissor::Instance();
	if (rs->Empty() && !params.view_region.IsValid()) {
		return false;
	}

	sm::rect r = spr->GetSymbol()->GetBounding(spr);
	S2_MAT mat = spr->GetLocalMat() * params.mt;
	sm::vec2 r_min = mat * sm::vec2(r.xmin, r.ymin);
	sm::vec2 r_max = mat * sm::vec2(r.xmax, r.ymax);
	sm::rect sr(r_min, r_max);

	if (!rs->Empty() && rs->IsOutside(sr)) {
		return true;
	}
	if (params.view_region.IsValid() && !sm::is_rect_intersect_rect(params.view_region, sr)) {
		return true;
	}
	return false;
}

}