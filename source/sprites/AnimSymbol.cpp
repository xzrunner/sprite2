#include "AnimSymbol.h"
#include "SymType.h"
#include "AnimSprite.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "AnimLerp.h"
#include "DrawNode.h"
#include "SymbolVisitor.h"

#include <assert.h>

namespace s2
{

AnimSymbol::AnimSymbol()
	: m_fps(30)
{
}

AnimSymbol::AnimSymbol(uint32_t id)
	: Symbol(id)
	, m_fps(30)
{
}

AnimSymbol::~AnimSymbol()
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			Frame* frame = layer->frames[j];
			for_each(frame->sprs.begin(), frame->sprs.end(), cu::RemoveRefFunctor<Sprite>());
			delete frame;
		}
		delete layer;
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

void AnimSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{	
	if (spr->GetID() == 2774) {
		int zz = 0;
	}

	if (spr) {
		RenderParams rp_child;
		if (DrawNode::Prepare(rp, spr, rp_child)) {
			const AnimSprite* anim = VI_DOWNCASTING<const AnimSprite*>(spr);
			const AnimCurr& curr = anim->GetAnimCurr(rp.actor);
			curr.Draw(rp_child);
		}
	} else {
		m_curr.Draw(rp);
	}
}

bool AnimSymbol::Update(const RenderParams& rp, float time)
{
	m_curr.SetTime(time);
	return m_curr.Update(rp);
}

sm::rect AnimSymbol::GetBounding(const Sprite* spr) const
{
	if (m_size.IsValid()) {
		return m_size;
	}

	m_size.MakeEmpty();
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				frame->sprs[k]->GetBounding()->CombineTo(m_size);
			}
		}
	}
	return m_size;
}

int AnimSymbol::GetMaxFrameIdx() const
{
	int index = 0;
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		AnimSymbol::Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			index = std::max(index, layer->frames[j]->index);
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
			float process = (float) (frame - curr_f->index) / (next_f->index - curr_f->index);
			AnimLerp::Lerp(curr_f->sprs, next_f->sprs, sprs, process, curr_f->lerps);
		}
	}
}

void AnimSymbol::LoadCopy()
{
	m_copy.LoadFromSym(*this);
	m_curr.SetAnimCopy(&m_copy);
}

void AnimSymbol::AddLayer(Layer* layer)
{
	m_layers.push_back(layer);

	m_size.MakeEmpty();
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
	m_curr.Clear();
	m_size.MakeEmpty();
	return dirty;	
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