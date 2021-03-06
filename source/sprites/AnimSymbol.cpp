#include "sprite2/AnimSymbol.h"
#include "sprite2/AnimCopy.h"
#include "sprite2/SymType.h"
#include "sprite2/AnimSprite.h"
#include "sprite2/Sprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/AnimLerp.h"
#include "sprite2/DrawNode.h"
#include "sprite2/SymbolVisitor.h"
#include "sprite2/Actor.h"
#include "sprite2/AnimActor.h"
#include "sprite2/AABBHelper.h"
#include "sprite2/Utility.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatTopNodes.h"
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS
#include "sprite2/ILerp.h"
#include "sprite2/AnimCurr.h"
#include "sprite2/UpdateParams.h"

#include <memmgr/Allocator.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

#include <assert.h>

namespace s2
{

AnimSymbol::AnimSymbol()
	: m_fps(30)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

AnimSymbol::AnimSymbol(uint32_t id)
	: Symbol(id)
	, m_fps(30)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

AnimSymbol::~AnimSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

int AnimSymbol::Type() const 
{ 
	return SYM_ANIMATION; 
}

void AnimSymbol::Traverse(const SymbolVisitor& visitor)
{
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) {
		auto& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer->frames.size(); iframe < nframe; ++iframe) {
			const auto& frame = layer->frames[iframe];
			for (int ispr = 0, nspr = frame->sprs.size(); ispr < nspr; ++ispr) {
				visitor.Visit(frame->sprs[ispr]);
			}
		}
	}
}

pt2::RenderReturn AnimSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{	
	pt2::RenderReturn ret = pt2::RENDER_OK;
	if (spr)
	{
#ifndef S2_DISABLE_STATISTICS
		int id = spr->GetSymbol()->GetID();
		//StatTopNodes::Checkpoint cp(id, rp.parent_id, rp.level);
		StatSymDraw::Instance()->AddDrawCount(STAT_SYM_ANIMATION);
		//	StatSymDraw::DrawCostCP cp2(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS

		RenderParamsProxy rp_proxy;
		RenderParams* rp_child = rp_proxy.obj;
		memcpy(rp_child, &rp, sizeof(rp));

#ifndef S2_DISABLE_STATISTICS
		rp_child->parent_id = id;
		rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS

		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			auto anim = S2_VI_DOWN_CAST<const AnimSprite*>(spr);
			auto& curr = anim->GetDrawCurr(rp.actor);
			ret = curr.Draw(dlist, *rp_child);
		}
	}
	else
	{
		ret = m_state.GetOrigin().Draw(dlist, rp);
	}
	return ret;
}

bool AnimSymbol::Update(const UpdateParams& up, float time)
{
	auto& curr = m_state.GetOrigin();
	curr.SetTime(time);
	return curr.Update(up, this, NULL);
}

int AnimSymbol::GetMaxFrameIdx() const
{
	int index = 0;
	for (auto& layer : m_layers) {
		if (!layer->frames.empty()) {
			index = std::max(index, layer->frames.back()->index);
		}
	}
	return index;
}

void AnimSymbol::CreateFrameSprites(int frame, CU_VEC<SprPtr>& sprs) const
{
	for (auto& layer : m_layers)
	{
		int curr = layer->GetCurrFrame(frame);
		int next = layer->GetNextFrame(frame);
		if (curr < 0) {
			continue;
		}
		const auto& curr_f = layer->frames[curr];
		if (!curr_f->tween || next < 0)
		{
			for (auto& spr : curr_f->sprs) {
				sprs.push_back(spr->Clone());
			}
		}
		else
		{
			const auto& next_f = layer->frames[next];
			assert(frame >= curr_f->index && frame < next_f->index);
			AnimLerp::Lerp(curr_f->sprs, next_f->sprs, sprs, frame - curr_f->index,
				next_f->index - curr_f->index, curr_f->lerps);
		}
	}
}

const std::shared_ptr<AnimCopy>& AnimSymbol::GetCopy() const
{
	if (!m_copy) {
		m_copy = CU_MAKE_SHARED<AnimCopy>();
		m_copy->LoadFromSym(*this);
	}
	return m_copy;
}

void AnimSymbol::LoadCopy()
{
	if (!m_copy) {
		m_copy = CU_MAKE_SHARED<AnimCopy>();
	}
	m_copy->LoadFromSym(*this);
}

void AnimSymbol::BuildCurr()
{
	m_state.Init(m_copy);
}

void AnimSymbol::AddLayer(LayerPtr& layer, int idx)
{
	if (idx < 0) {
		m_layers.push_back(std::move(layer));		
	} else {
		m_layers.insert(m_layers.begin() + idx, std::move(layer));
	}

	m_aabb.MakeEmpty();
}

bool AnimSymbol::Clear()
{
	bool dirty = false;
	m_layers.clear();
		
	m_aabb.MakeEmpty();
	return dirty;	
}

sm::rect AnimSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (!cache) {
		return CalcAABB(spr, actor);
	}

	if (!m_aabb.IsValid()) {
		m_aabb = CalcAABB(spr, actor);
	}
	return m_aabb;
}

sm::rect AnimSymbol::CalcAABB(const Sprite* spr, const Actor* actor) const
{
	if (actor) {
		auto anim_actor = static_cast<const AnimActor*>(actor);
		return anim_actor->GetState().GetOrigin().CalcAABB(actor);
	}

	CU_VEC<SprPtr> children;
	int num = 0;
	for (auto& layer : m_layers) {
		for (auto& frame : layer->frames) {
			num += frame->sprs.size();
		}
	}
	children.reserve(num);
	sm::rect aabb;
	for (auto& layer : m_layers) {
		for (auto& frame : layer->frames) {
			for (auto& spr : frame->sprs) {
				children.push_back(spr);			
			}
		}
	}
	return AABBHelper::CalcAABB(children, actor);
}

/************************************************************************/
/* class AnimSymbol::Layer                                              */
/************************************************************************/

int AnimSymbol::Layer::GetCurrFrame(int index) const
{
	if (frames.empty()) {
		return -1;
	}

	int prev = -1, curr = -1;
	int idx = 0;
	for (auto& frame : frames)
	{
		if (frame->index >= index) {
			curr = idx;
			break;
		} else {
			prev = idx;
		}
		++idx;
	}
	
	if (curr < 0) {
		return -1;
	} else if (curr == index) {
		return curr;
	} else if (prev < 0) {
		return -1;
	} else {
		assert(prev <= index);
		return prev;
	}
}

int AnimSymbol::Layer::GetNextFrame(int index) const
{
	int idx = 0;
	for (auto& frame : frames) {
		if (frame->index > index) {
			return idx;
		}
		++idx;
	}
	return -1;
}

}
