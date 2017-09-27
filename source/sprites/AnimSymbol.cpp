#include "AnimSymbol.h"
#include "AnimCopy.h"
#include "SymType.h"
#include "AnimSprite.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "AnimLerp.h"
#include "DrawNode.h"
#include "SymbolVisitor.h"
#include "S2_Actor.h"
#include "AnimActor.h"
#include "AABBHelper.h"
#include "sprite2/Utility.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatTopNodes.h"
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS
#include "ILerp.h"
#include "sprite2/AnimTreeCurr.h"
#include "sprite2/UpdateParams.h"

#include <assert.h>

namespace s2
{

AnimSymbol::AnimSymbol()
	: m_fps(30)
	, m_curr(NULL)
	, m_copy(NULL)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

AnimSymbol::AnimSymbol(uint32_t id)
	: Symbol(id)
	, m_fps(30)
	, m_curr(NULL)
	, m_copy(NULL)
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

	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			Frame* frame = layer->frames[j];
			for_each(frame->sprs.begin(), frame->sprs.end(), cu::RemoveRefFunctor<Sprite>());
			delete frame;
		}
		delete layer;
	}

	if (m_curr) {
		m_curr->RemoveReference();
	}
	if (m_copy) {
		delete m_copy;
	}
}

int AnimSymbol::Type() const 
{ 
	return SYM_ANIMATION; 
}

void AnimSymbol::Traverse(const SymbolVisitor& visitor)
{
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) {
		Layer* layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer->frames.size(); iframe < nframe; ++iframe) {
			Frame* frame = layer->frames[iframe];
			for (int ispr = 0, nspr = frame->sprs.size(); ispr < nspr; ++ispr) {
				visitor.Visit(frame->sprs[ispr]);
			}
		}
	}
}

RenderReturn AnimSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{	
#ifndef S2_DISABLE_STATISTICS
	int id = -1;
	if (spr) {
		id = spr->GetSymbol()->GetID();
	}
	StatTopNodes::Checkpoint cp(id, rp.parent_id, rp.level);
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_ANIMATION);
//	StatSymDraw::DrawCostCP cp2(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS

	RenderReturn ret = RENDER_OK;
	if (spr) {
		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
#ifndef S2_DISABLE_STATISTICS
		rp_child->parent_id = id;
		rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS
		if (DrawNode::Prepare(rp, spr, *rp_child)) {
			const AnimSprite* anim = VI_DOWNCASTING<const AnimSprite*>(spr);
			const AnimTreeCurr* curr = anim->GetAnimCurr(rp.actor);
			assert(curr);
			ret = curr->Draw(*rp_child);
		}
		RenderParamsPool::Instance()->Push(rp_child); 
	} else {
		ret = m_curr->Draw(rp);
	}
	return ret;
}

bool AnimSymbol::Update(const UpdateParams& up, float time)
{
	m_curr->SetTime(time);
	return m_curr->Update(up, this, NULL);
}

int AnimSymbol::GetMaxFrameIdx() const
{
	int index = 0;
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		AnimSymbol::Layer* layer = m_layers[i];
		if (!layer->frames.empty()) {
			index = std::max(index, layer->frames.back()->index);
		}
	}
	return index;
}

void AnimSymbol::CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		Frame *curr_f = layer->GetCurrFrame(frame),
			  *next_f = layer->GetNextFrame(frame);
		if (!curr_f)
			continue;
		if (!curr_f->tween || !next_f)
		{
			for (int i = 0, n = curr_f->sprs.size(); i < n; ++i) {
				Sprite* spr = VI_CLONE(Sprite, curr_f->sprs[i]);
				sprs.push_back(spr);	
			}
		}
		else
		{
			assert(frame >= curr_f->index && frame < next_f->index);
			AnimLerp::Lerp(curr_f->sprs, next_f->sprs, sprs, frame - curr_f->index, 
				next_f->index - curr_f->index, curr_f->lerps);
		}
	}
}

const AnimCopy* AnimSymbol::GetCopy() const
{
	if (!m_copy) {
		m_copy = new AnimCopy;
		m_copy->LoadFromSym(*this);
	}
	return m_copy;
}

void AnimSymbol::LoadCopy()
{
	if (!m_copy) {
		m_copy = new AnimCopy;
	}
	m_copy->LoadFromSym(*this);
}

void AnimSymbol::BuildCurr()
{
	if (!m_curr) {
		m_curr = new AnimTreeCurr();
		m_curr->SetAnimCopy(GetCopy());
		m_curr->Start(UpdateParams(), nullptr);
	}
}

void AnimSymbol::AddLayer(Layer* layer, int idx)
{
	if (idx < 0) {
		m_layers.push_back(layer);		
	} else {
		m_layers.insert(m_layers.begin() + idx, layer);
	}

	m_aabb.MakeEmpty();
}

bool AnimSymbol::Clear()
{
	bool dirty = false;
 	for (int i = 0, n = m_layers.size(); i < n; ++i) {
 		Layer* layer = m_layers[i];
 		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
 			Frame* frame = layer->frames[j];
			for_each(frame->sprs.begin(), frame->sprs.end(), cu::RemoveRefFunctor<Sprite>());
			delete frame;
 		}
		delete layer;
 	}
	m_layers.clear();
	m_curr->Clear();
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
		const AnimActor* anim_actor = static_cast<const AnimActor*>(actor);
		return anim_actor->GetCurr()->CalcAABB(actor);
	}

	std::vector<Sprite*> children;
	int num = 0;
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			Frame* frame = layer->frames[j];
			num += frame->sprs.size();
		}
	}
	children.reserve(num);
	sm::rect aabb;
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				children.push_back(frame->sprs[k]);			
			}
		}
	}
	return AABBHelper::CalcAABB(children, actor);
}

/************************************************************************/
/* class AnimSymbol::Frame                                              */
/************************************************************************/

AnimSymbol::Frame::~Frame()
{
	for (int i = 0, n = lerps.size(); i < n; ++i) {
		delete lerps[i].second;
	}
}

/************************************************************************/
/* class AnimSymbol::Layer                                              */
/************************************************************************/

AnimSymbol::Frame* AnimSymbol::Layer::GetCurrFrame(int index) const
{
	if (frames.empty()) {
		return NULL;
	}

	AnimSymbol::Frame *prev = NULL, *curr = NULL;
	for (int i = 0, n = frames.size(); i < n; ++i)
	{
		AnimSymbol::Frame* frame = frames[i];
		if (frame->index >= index) {
			curr = frame;
			break;
		} else {
			prev = frame;
		}
	}
	
	if (!curr) {
		return NULL;
	} else if (curr->index == index) {
		return curr;
	} else if (!prev) {
		return NULL;
	} else {
		assert(prev->index <= index);
		return prev;
	}
}

AnimSymbol::Frame* AnimSymbol::Layer::GetNextFrame(int index) const
{
	for (int i = 0, n = frames.size(); i < n; ++i)
	{
		AnimSymbol::Frame* frame = frames[i];
		if (frame->index > index) {
			return frame;
		}
	}
	return NULL;
}

}
