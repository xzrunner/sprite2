#include "AnimCopy.h"
#include "AnimSymbol.h"
#include "AnimTreeCurr.h"
#include "S2_Sprite.h"
#include "RenderColor.h"
#include "Flatten.h"
#include "FlattenParams.h"
#include "AnimFlatten.h"

#include <assert.h>

namespace s2
{

AnimCopy::AnimCopy()
	: m_max_frame_idx(0)
	, m_max_item_num(0)
{
}

AnimCopy::~AnimCopy()
{
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

void AnimCopy::LoadFromSym(const AnimSymbol& sym)
{
	SetCountNum(sym);
	FillingLayers(sym);
	ConnectItems(sym);
	LoadLerpData(sym);
	CreateSprSlots(sym);
}

void AnimCopy::StoreToFlatten(AnimFlatten& ft, const Actor* actor) const
{
	S2_MAT prev_mat;
	if (actor) {
		Utility::PrepareMat(S2_MAT(), actor->GetSpr(), actor, prev_mat);
	}

	// prepare slots
	std::vector<Sprite*> slots;
	slots.resize(m_slots.size());
	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		slots[i] = VI_CLONE(Sprite, m_slots[i]);
	}
	
	// filling frames
	std::vector<Flatten> frames;
	frames.reserve(m_max_frame_idx);
	for (int i = 0; i < m_max_frame_idx; ++i)
	{
		int time = i + 1;
		Flatten frame;
		for (int j = 0, m = m_layers.size(); j < m; ++j)
		{
			const Layer& layer = m_layers[j];
			if (layer.frames.empty()) {
				continue;
			}
			// find curr frame
			const Frame* curr = NULL;
			int curr_frame = -1;
			for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) {				
				const Frame& frame = layer.frames[iframe];
				if (frame.time == time) {
					curr = &frame;
					curr_frame = iframe;
					break;
				} else if (frame.time > time) {
					break;
				} else if (iframe < nframe - 1) {
					const Frame& next = layer.frames[iframe + 1];
					if (next.time > time) {
						curr = &frame;
						curr_frame = iframe;
					}
				}
			}
			if (!curr) {
				continue;
			}

			// filling
			for (int iitem = 0, nitem = curr->items.size(); iitem < nitem; ++iitem) 
			{
				const Item& item = curr->items[iitem];
				const Sprite* spr = NULL;
				if (item.next != -1)
				{
					assert(item.lerp != -1);
					const Frame& next_frame = layer.frames[curr_frame + 1];
					assert(item.slot == next_frame.items[item.next].slot);
					Sprite* tween = slots[item.slot];
					AnimTreeCurr::LoadSprLerpData(tween, m_lerps[item.lerp], time - curr->time);
					// todo AnimLerp::LerpSpecial
					spr = tween;
				}
				else
				{
					spr = item.spr;
				}

				FlattenParams fp;
				fp.SetPrevMat(prev_mat);
				fp.SetSpr(spr);
				fp.SetActor(spr->QueryActor(actor));
				spr->GetSymbol()->Flattening(fp, frame);	
			}
		}
		frames.push_back(frame);
	}
	ft.SetFrames(frames);

	// release slots
	for_each(slots.begin(), slots.end(), cu::RemoveRefFunctor<Sprite>());
}

void AnimCopy::SetCountNum(const AnimSymbol& sym)
{
	m_max_frame_idx = sym.GetMaxFrameIdx();

	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) 
	{
		int max_count = -1;
		for (int j = 0, m = layers[i]->frames.size(); j < m; ++j) 
		{
			const AnimSymbol::Frame* frame = layers[i]->frames[j];
			int count = frame->sprs.size();
			if (count > max_count) {
				max_count = count;
			}
		}
		m_max_item_num += max_count;
	}
}

void AnimCopy::FillingLayers(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	m_layers.clear();
	m_layers.resize(layers.size());
	for (int ilayer = 0, nlayer = layers.size(); ilayer < nlayer; ++ilayer) 
	{
		const AnimSymbol::Layer* src_layer = layers[ilayer];
		Layer& dst_layer = m_layers[ilayer];
		dst_layer.frames.resize(src_layer->frames.size());
		for (int iframe = 0, nframe = src_layer->frames.size(); iframe < nframe; ++iframe)
		{
			const AnimSymbol::Frame* src_frame = src_layer->frames[iframe];
			Frame& dst_frame = dst_layer.frames[iframe];
			dst_frame.time = src_frame->index;
			dst_frame.items.resize(src_frame->sprs.size());
			for (int iitem = 0, nitem = src_frame->sprs.size(); iitem < nitem; ++iitem) {
				const Sprite* src_spr = src_frame->sprs[iitem];
				src_spr->AddReference();
				dst_frame.items[iitem].spr = src_spr;
			}
		}
	}
}

void AnimCopy::ConnectItems(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		if (layer.frames.size() <= 1) {
			continue;
		}
		const AnimSymbol::Layer* src_layer = layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe - 1; ++iframe) 
		{
			if (!src_layer->frames[iframe]->tween) {
				continue;
			}

			Frame& curr = layer.frames[iframe];
			Frame& next = layer.frames[iframe + 1];
			for (int icurr = 0, ncurr = curr.items.size(); icurr < ncurr; ++icurr) {
				for (int inext = 0, nnext = next.items.size(); inext < nnext; ++inext) {
					const Sprite* curr_spr = src_layer->frames[iframe]->sprs[icurr];
					const Sprite* next_spr = src_layer->frames[iframe+1]->sprs[inext];
					if (curr_spr->GetName() == next_spr->GetName()) {
						curr.items[icurr].next = inext;
						next.items[inext].prev = icurr;
						break;
					}
				}
			}
		}
	}
}

void AnimCopy::LoadLerpData(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) 
		{
			Frame& frame = layer.frames[iframe];
			for (int iitem = 0, nitem = frame.items.size(); iitem < nitem; ++iitem) 
			{
				Item& item = frame.items[iitem];
				if (item.next == -1) {
					continue;
				}
				assert(item.lerp == -1);

				int idx = m_lerps.size();

				Lerp dst;

				const Sprite* begin = layers[ilayer]->frames[iframe]->sprs[iitem];
				const Sprite* end = layers[ilayer]->frames[iframe+1]->sprs[item.next];
				int dt = layer.frames[iframe + 1].time - layer.frames[iframe].time;

				SprSRT bsrt, esrt;
				begin->GetLocalSRT(bsrt);
				end->GetLocalSRT(esrt);
				dst.srt = bsrt;
				for (int i = 0; i < SprSRT::SRT_MAX; ++i) {
					dst.dsrt.srt[i] = (esrt.srt[i] - bsrt.srt[i]) / dt;
				}

				const RenderColor& rcb = begin->GetColor();
				const RenderColor& rce = end->GetColor();
				dst.col_mul = rcb.GetMul();
				dst.col_add = rcb.GetAdd();
				CalcDeltaColor(rcb.GetMul(), rce.GetMul(), dt, dst.dcol_mul);
				CalcDeltaColor(rcb.GetAdd(), rce.GetAdd(), dt, dst.dcol_add);

				m_lerps.push_back(dst);
				item.lerp = idx;
			}
		}
	}
}

void AnimCopy::CreateSprSlots(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) 
		{
			Frame& frame = layer.frames[iframe];
			for (int iitem = 0, nitem = frame.items.size(); iitem < nitem; ++iitem) 
			{
				Item& item = frame.items[iitem];
				if (item.slot != -1) {
					continue;
				}
				int slot = m_slots.size();
				const Sprite* spr = VI_CLONE(Sprite, layers[ilayer]->frames[iframe]->sprs[iitem]);
				m_slots.push_back(spr);
				item.slot = slot;

				Item* ptr = &item;
				int idx = iframe;
				while (ptr->next != -1 && idx < nframe - 1) 
				{
					ptr = &layer.frames[++idx].items[ptr->next];
					ptr->slot = slot;
				}
			}
		}
	}
}

void AnimCopy::CalcDeltaColor(const Color& begin, const Color& end, int time, float* ret)
{
	ret[0] = (end.r - begin.r) / (float)time;
	ret[1] = (end.g - begin.g) / (float)time;
	ret[2] = (end.b - begin.b) / (float)time;
	ret[3] = (end.a - begin.a) / (float)time;
}

/************************************************************************/
/* struct AnimCopy::Item                                                */
/************************************************************************/

AnimCopy::Item::Item() 
	: slot(-1)
	, next(-1)
	, prev(-1)
	, lerp(-1)
	, spr(NULL) 
{}

AnimCopy::Item::~Item() 
{
	if (spr) {
		spr->RemoveReference();
	}
}

}