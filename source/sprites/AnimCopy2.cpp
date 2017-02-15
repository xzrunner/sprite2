#include "AnimCopy2.h"
#include "AnimSymbol.h"
#include "S2_Sprite.h"

#include <assert.h>

namespace s2
{

AnimCopy2::AnimCopy2()
	: m_max_frame_idx(0)
	, m_max_node_num(0)
{
}

AnimCopy2::~AnimCopy2()
{
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

void AnimCopy2::LoadFromSym(const AnimSymbol& sym)
{
	m_layers.clear();


	// count
	const std::vector<AnimSymbol::Layer*>& src_layers 
		= VI_DOWNCASTING<const AnimSymbol&>(sym).GetLayers();
	for (int i = 0, n = src_layers.size(); i < n; ++i) {
		int max_count = -1;
		for (int j = 0, m = src_layers[i]->frames.size(); j < m; ++j) {
			const AnimSymbol::Frame* frame = src_layers[i]->frames[j];
		 	if (frame->index > m_max_frame_idx) {
		 		m_max_frame_idx = frame->index;
		 	}
			int count = frame->sprs.size();
			if (count > max_count) {
				max_count = count;
			}
		}
		m_max_node_num += max_count;
	}

	// filling
	m_layers.resize(src_layers.size());
	for (int ilayer = 0, nlayer = src_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		const AnimSymbol::Layer* src_layer = src_layers[ilayer];
		Layer& dst_layer = m_layers[ilayer];
		dst_layer.frames.resize(src_layer->frames.size());
		for (int iframe = 0, nframe = src_layer->frames.size(); iframe < nframe; ++iframe)
		{
			const AnimSymbol::Frame* src_frame = src_layer->frames[iframe];
			Frame& dst_frame = dst_layer.frames[iframe];
			dst_frame.time = src_frame->index;
			dst_frame.nodes.resize(src_frame->sprs.size());
		}
	}

	// connect
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) 
	{
		Layer& layer = m_layers[ilayer];
		if (layer.frames.size() <= 1) {
			continue;
		}
		const AnimSymbol::Layer* src_layer = src_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe - 1; ++iframe) 
		{
			if (!src_layer->frames[iframe]->tween) {
				continue;
			}

			Frame& curr = layer.frames[iframe];
			Frame& next = layer.frames[iframe + 1];
			for (int icurr = 0, ncurr = curr.nodes.size(); icurr < ncurr; ++icurr) {
				for (int inext = 0, nnext = next.nodes.size(); inext < nnext; ++inext) {
					const Sprite* curr_spr = src_layer->frames[iframe]->sprs[icurr];
					const Sprite* next_spr = src_layer->frames[iframe+1]->sprs[inext];
					if (curr_spr->GetSymbol() == next_spr->GetSymbol() &&
						curr_spr->GetName() == next_spr->GetName()) {
						curr.nodes[icurr].next = inext;
						next.nodes[inext].prev = icurr;
						break;
					}
				}
			}
		}
	}

	// lerp data
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) {
		Layer& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) {
			Frame& frame = layer.frames[iframe];
			for (int inode = 0, nnode = frame.nodes.size(); inode < nnode; ++inode) {
				Node& node = frame.nodes[inode];
				if (node.next == -1) {
					continue;
				}
				assert(node.lerp_data == -1);
				int idx = m_lerp_data.size();

				LerpData dst;
				const Sprite* begin = src_layers[ilayer]->frames[iframe]->sprs[inode];
				const Sprite* end = src_layers[ilayer]->frames[iframe+1]->sprs[inode];
				int dt = layer.frames[iframe + 1].time - layer.frames[iframe].time;
				dst.pos = begin->GetPosition();
				dst.dpos = (end->GetPosition() - dst.pos) / dt;

				m_lerp_data.push_back(dst);
				node.lerp_data = idx;
			}
		}
	}

	// sprite slot
	for (int ilayer = 0, nlayer = m_layers.size(); ilayer < nlayer; ++ilayer) {
		Layer& layer = m_layers[ilayer];
		for (int iframe = 0, nframe = layer.frames.size(); iframe < nframe; ++iframe) {
			Frame& frame = layer.frames[iframe];
			for (int inode = 0, nnode = frame.nodes.size(); inode < nnode; ++inode) {
				Node& node = frame.nodes[inode];
				if (node.curr != -1) {
					continue;
				}
				int slot = m_slots.size();
				const Sprite* spr = VI_CLONE(Sprite, src_layers[ilayer]->frames[iframe]->sprs[inode]);
				m_slots.push_back(spr);
				node.curr = slot;

				Node* ptr = &node;
				int idx = iframe;
				while (ptr->next != -1 && idx < nframe - 1) 
				{
					ptr = &layer.frames[++idx].nodes[ptr->next];
					ptr->curr = slot;
				}
			}
		}
	}
}

}