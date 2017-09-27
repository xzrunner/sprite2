#include "AnimCopy.h"
#include "AnimSymbol.h"
#include "S2_Sprite.h"
#include "RenderColor.h"
#include "ILerp.h"

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

			dst_frame.lerps.reserve(src_frame->lerps.size());
			for (int ilerp = 0, nlerp = src_frame->lerps.size(); ilerp < nlerp; ++ilerp) {
				AnimLerp::SprData type = src_frame->lerps[ilerp].first;
				ILerp* lerp = src_frame->lerps[ilerp].second->Clone();
				dst_frame.lerps.push_back(std::make_pair(type, lerp));
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

/************************************************************************/
/* struct AnimCopy::Frame                                               */
/************************************************************************/

AnimCopy::Frame::~Frame()
{
	for (int i = 0, n = lerps.size(); i < n; ++i) {
		delete lerps[i].second;
	}
	lerps.clear();
}

}