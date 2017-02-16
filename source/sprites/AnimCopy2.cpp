#include "AnimCopy2.h"
#include "AnimSymbol.h"
#include "S2_Sprite.h"
#include "RenderColor.h"

#include <assert.h>

namespace s2
{

AnimCopy2::AnimCopy2()
	: m_max_frame_idx(0)
	, m_max_actor_num(0)
{
}

AnimCopy2::~AnimCopy2()
{
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

void AnimCopy2::LoadFromSym(const AnimSymbol& sym)
{
	SetCountNum(sym);
	FillingLayers(sym);
	ConnectActors(sym);
	LoadLerpData(sym);
	CreateSprSlots(sym);
}

void AnimCopy2::SetCountNum(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) 
	{
		int max_count = -1;
		for (int j = 0, m = layers[i]->frames.size(); j < m; ++j) 
		{
			const AnimSymbol::Frame* frame = layers[i]->frames[j];
			if (frame->index > m_max_frame_idx) {
				m_max_frame_idx = frame->index;
			}
			int count = frame->sprs.size();
			if (count > max_count) {
				max_count = count;
			}
		}
		m_max_actor_num += max_count;
	}
}

void AnimCopy2::FillingLayers(const AnimSymbol& sym)
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
			dst_frame.actors.resize(src_frame->sprs.size());
			for (int iactor = 0, nactor = src_frame->sprs.size(); iactor < nactor; ++iactor) {
				const Sprite* src_spr = src_frame->sprs[iactor];
				src_spr->AddReference();
				dst_frame.actors[iactor].spr = src_spr;
			}
		}
	}
}

void AnimCopy2::ConnectActors(const AnimSymbol& sym)
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
			for (int icurr = 0, ncurr = curr.actors.size(); icurr < ncurr; ++icurr) {
				for (int inext = 0, nnext = next.actors.size(); inext < nnext; ++inext) {
					const Sprite* curr_spr = src_layer->frames[iframe]->sprs[icurr];
					const Sprite* next_spr = src_layer->frames[iframe+1]->sprs[inext];
					if (curr_spr->GetSymbol() == next_spr->GetSymbol() &&
						curr_spr->GetName() == next_spr->GetName()) {
						curr.actors[icurr].next = inext;
						next.actors[inext].prev = icurr;
						break;
					}
				}
			}
		}
	}
}

void AnimCopy2::LoadLerpData(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) 
		{
			Frame& frame = layer.frames[iframe];
			for (int iactor = 0, nactor = frame.actors.size(); iactor < nactor; ++iactor) 
			{
				Actor& actor = frame.actors[iactor];
				if (actor.next == -1) {
					continue;
				}
				assert(actor.lerp == -1);

				int idx = m_lerps.size();

				Lerp dst;

				const Sprite* begin = layers[ilayer]->frames[iframe]->sprs[iactor];
				const Sprite* end = layers[ilayer]->frames[iframe+1]->sprs[actor.next];
				int dt = layer.frames[iframe + 1].time - layer.frames[iframe].time;

				const SprSRT& bsrt = begin->GetLocalSRT();
				const SprSRT& esrt = end->GetLocalSRT();
				dst.srt = bsrt;
				dst.dsrt.position = (esrt.position - bsrt.position) / dt;
				dst.dsrt.angle    = (esrt.angle - bsrt.angle) / dt;
				dst.dsrt.scale    = (esrt.scale - bsrt.scale) / dt;
				dst.dsrt.shear    = (esrt.shear - bsrt.shear) / dt;
				dst.dsrt.offset   = (esrt.offset - bsrt.offset) / dt;
				dst.dsrt.center   = (esrt.center - bsrt.center) / dt;

				const RenderColor& rcb = begin->GetColor();
				const RenderColor& rce = end->GetColor();
				dst.col_mul = rcb.GetMul();
				dst.col_add = rce.GetAdd();
				CalcDeltaColor(rcb.GetMul(), rce.GetMul(), dt, dst.dcol_mul);
				CalcDeltaColor(rcb.GetAdd(), rce.GetAdd(), dt, dst.dcol_add);

				m_lerps.push_back(dst);
				actor.lerp = idx;
			}
		}
	}
}

void AnimCopy2::CreateSprSlots(const AnimSymbol& sym)
{
	const std::vector<AnimSymbol::Layer*>& layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) 
		{
			Frame& frame = layer.frames[iframe];
			for (int iactor = 0, nactor = frame.actors.size(); iactor < nactor; ++iactor) 
			{
				Actor& actor = frame.actors[iactor];
				if (actor.slot != -1) {
					continue;
				}
				int slot = m_slots.size();
				const Sprite* spr = VI_CLONE(Sprite, layers[ilayer]->frames[iframe]->sprs[iactor]);
				m_slots.push_back(spr);
				actor.slot = slot;

				Actor* ptr = &actor;
				int idx = iframe;
				while (ptr->next != -1 && idx < nframe - 1) 
				{
					ptr = &layer.frames[++idx].actors[ptr->next];
					ptr->slot = slot;
				}
			}
		}
	}
}

void AnimCopy2::CalcDeltaColor(const Color& begin, const Color& end, int time, float* ret)
{
	ret[0] = (end.r - begin.r) / (float)time;
	ret[1] = (end.g - begin.g) / (float)time;
	ret[2] = (end.b - begin.b) / (float)time;
	ret[3] = (end.a - begin.a) / (float)time;
}

/************************************************************************/
/* struct AnimCopy2::Actor                                              */
/************************************************************************/

AnimCopy2::Actor::Actor() 
	: slot(-1)
	, next(-1)
	, prev(-1)
	, lerp(-1)
	, spr(NULL) 
{}

AnimCopy2::Actor::~Actor() 
{
	if (spr) {
		spr->RemoveReference();
	}
}

}