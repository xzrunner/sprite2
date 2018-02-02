#include "sprite2/AnimCopy.h"
#include "sprite2/AnimSymbol.h"
#include "sprite2/Sprite.h"
#include "sprite2/ILerp.h"

#include <assert.h>

namespace s2
{

AnimCopy::AnimCopy()
	: m_max_frame_idx(0)
	, m_max_item_num(0)
{
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

	auto& layers = sym.GetLayers();
	for (auto& layer : layers) 
	{
		int max_count = -1;
		for (auto& frame : layer->frames) 
		{
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
	auto& layers = sym.GetLayers();
	m_layers.clear();
	m_layers.resize(layers.size());
	for (int ilayer = 0, nlayer = layers.size(); ilayer < nlayer; ++ilayer) 
	{
		auto& src_layer = layers[ilayer];
		Layer& dst_layer = m_layers[ilayer];
		dst_layer.frames.resize(src_layer->frames.size());
		for (int iframe = 0, nframe = src_layer->frames.size(); iframe < nframe; ++iframe)
		{
			const auto& src_frame = src_layer->frames[iframe];
			Frame& dst_frame = dst_layer.frames[iframe];
			dst_frame.time = src_frame->index;

			dst_frame.items.resize(src_frame->sprs.size());
			for (int iitem = 0, nitem = src_frame->sprs.size(); iitem < nitem; ++iitem) {
				dst_frame.items[iitem].spr = src_frame->sprs[iitem];
			}

			dst_frame.lerps.reserve(src_frame->lerps.size());
			for (int ilerp = 0, nlerp = src_frame->lerps.size(); ilerp < nlerp; ++ilerp) {
				AnimLerp::SprData type = src_frame->lerps[ilerp].first;
				std::unique_ptr<ILerp> lerp = src_frame->lerps[ilerp].second->Clone();
				dst_frame.lerps.push_back(std::make_pair(type, std::move(lerp)));
			}
		}
	}
}

void AnimCopy::ConnectItems(const AnimSymbol& sym)
{
	auto& layers = sym.GetLayers();
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		if (layer.frames.size() <= 1) {
			continue;
		}
		auto& src_layer = layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe - 1; ++iframe) 
		{
			if (!src_layer->frames[iframe]->tween) {
				continue;
			}

			Frame& curr = layer.frames[iframe];
			Frame& next = layer.frames[iframe + 1];
			for (int icurr = 0, ncurr = curr.items.size(); icurr < ncurr; ++icurr) {
				for (int inext = 0, nnext = next.items.size(); inext < nnext; ++inext) {
					auto& curr_spr = src_layer->frames[iframe]->sprs[icurr];
					auto& next_spr = src_layer->frames[iframe+1]->sprs[inext];
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
	auto& layers = sym.GetLayers();
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

				auto& begin = layers[ilayer]->frames[iframe]->sprs[iitem];
				auto& end   = layers[ilayer]->frames[iframe+1]->sprs[item.next];
				int dt = layer.frames[iframe + 1].time - layer.frames[iframe].time;

				auto& bsrt = begin->GetLocalSRT();
				auto& esrt = end->GetLocalSRT();
				dst.srt = bsrt;
				dst.dsrt = (esrt - bsrt) / static_cast<float>(dt);

				auto& begin_col = begin->GetColorCommon();
				auto& end_col   = end->GetColorCommon();
				dst.col_mul = end_col.mul;
				dst.col_add = end_col.add;
				CalcDeltaColor(begin_col.mul, end_col.mul, dt, dst.dcol_mul);
				CalcDeltaColor(begin_col.add, end_col.add, dt, dst.dcol_add);

				m_lerps.push_back(dst);
				item.lerp = idx;
			}
		}
	}
}

void AnimCopy::CreateSprSlots(const AnimSymbol& sym)
{
	auto& layers = sym.GetLayers();
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
				
				m_slots.push_back(layers[ilayer]->frames[iframe]->sprs[iitem]->Clone());
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

void AnimCopy::CalcDeltaColor(const pt2::Color& begin, const pt2::Color& end, int time, float* ret)
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
{}

}