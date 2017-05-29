#include "AnimTreeCurr.h"
#include "AnimCopy.h"
#include "S2_Sprite.h"
#include "Animation.h"
#include "SpriteVisitor.h"
#include "DrawNode.h"
#include "AnimLerp.h"
#include "AnimLerp.h"
#include "SprVisitorParams.h"
#include "S2_Symbol.h"
#include "S2_Actor.h"
#include "ActorFactory.h"
#include "SymType.h"
#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "UpdateParams.h"
#include "ProxyHelper.h"
#include "SprVisitorParams.h"
#include "SetStaticFrameVisitor.h"
#include "AABBHelper.h"

#include <algorithm>
#include <climits>
#include <map>

#include <assert.h>
#include <string.h>
#include <fault.h>

namespace s2
{

AnimTreeCurr::AnimTreeCurr()
	: m_copy(NULL)
	, m_curr(NULL)
{
	ResetLayerCursor();
}

AnimTreeCurr::AnimTreeCurr(const AnimTreeCurr& curr)
	: m_copy(curr.m_copy)
	, m_layer_cursor(curr.m_layer_cursor)
	, m_layer_cursor_update(curr.m_layer_cursor_update)
	, m_slots(curr.m_slots)
	, m_curr_num(curr.m_curr_num)
{
	m_curr = new int[curr.m_copy->m_max_item_num];
	memcpy(m_curr, curr.m_curr, curr.m_curr_num * sizeof(int));

	for_each(m_slots.begin(), m_slots.end(), cu::AddRefFunctor<Sprite>());	
}

AnimTreeCurr& AnimTreeCurr::operator = (const AnimTreeCurr& curr)
{	
	m_copy = curr.m_copy;
	m_layer_cursor = curr.m_layer_cursor;
	m_layer_cursor_update = curr.m_layer_cursor_update;
	m_slots = curr.m_slots;
	for_each(m_slots.begin(), m_slots.end(), cu::AddRefFunctor<Sprite>());
	m_curr_num = curr.m_curr_num;
	m_curr = new int[curr.m_copy->m_max_item_num];
	memcpy(m_curr, curr.m_curr, curr.m_curr_num * sizeof(int));
	m_ctrl = curr.m_ctrl;
	return *this;
}

AnimTreeCurr::~AnimTreeCurr()
{
	delete m_curr;
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

AnimTreeCurr* AnimTreeCurr::Clone() const
{
	return new AnimTreeCurr(*this);
}

bool AnimTreeCurr::Update(const UpdateParams& up, const Sprite* spr, 
						  bool loop, float interval, int fps)
{
	if (!m_ctrl.IsActive()) {
		return UpdateChildren(up, spr);
	}

	if (!m_ctrl.Update()) {
		return false;
	}

	int curr_frame = UpdateFrameCursor(loop, interval, fps, true);

	bool dirty = false;

	// update curr frame
	if (curr_frame != m_ctrl.GetFrame()) 
	{
		m_ctrl.SetFrame(curr_frame);
		dirty = true;
		LoadCurrSprites(up, spr);
	}

	if (UpdateChildren(up, spr)) {
		dirty = true;
	}

	return dirty;
}

void AnimTreeCurr::SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps)
{
	int frame_copy = frame;
	frame = frame % (m_copy->m_max_frame_idx);

	if (frame < m_ctrl.GetFrame()) {
		ResetLayerCursor();
	}

	m_ctrl.SetFrame(frame, fps);

	LoadCurrSprites(up, spr);

	SetChildrenFrame(up, spr, frame_copy, fps);
}

void AnimTreeCurr::Start(const UpdateParams& up, const Sprite* spr)
{
	m_ctrl.Reset();
	m_ctrl.SetFrame(0);

	ResetLayerCursor();
	LoadCurrSprites(up, spr);
}

void AnimTreeCurr::OnMessage(const UpdateParams& up, const Sprite* spr, Message msg)
{
	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);
	for (int i = 0; i < m_curr_num; ++i) 
	{
		Sprite* child = m_slots[m_curr[i]];
		up_child->SetActor(child->QueryActor(up.GetActor()));
		child->OnMessage(*up_child, msg);
	}
	UpdateParamsPool::Instance()->Push(up_child); 
}

Sprite* AnimTreeCurr::FetchChildByName(int name, const Actor* actor) const
{
	std::vector<std::pair<const Actor*, Sprite*> > group;
	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		Sprite* child = m_slots[i];
		if (child->GetName() == name) {
			group.push_back(std::make_pair(actor, child));
		}
	}
	return ProxyHelper::BuildGroup(group);
}

Sprite* AnimTreeCurr::FetchChildByIdx(int idx) const
{
	if (idx < 0 || idx >= m_slots.size()) {
		return NULL;
	} else {
		return m_slots[idx];
	}
}

VisitResult AnimTreeCurr::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	if (m_curr_num == 0) {
		return ret;
	}

	SprVisitorParams cp = params;
	if (visitor.GetOrder()) {
		for (int i = 0; i < m_curr_num; ++i) 
		{
			Sprite* child = m_slots[m_curr[i]];
			cp.actor = child->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	} else {
		for (int i = m_curr_num - 1; i >= 0; --i) 
		{
			Sprite* child = m_slots[m_curr[i]];
			cp.actor = child->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	}
	return ret;
}

RenderReturn AnimTreeCurr::Draw(const RenderParams& rp) const
{
	RenderReturn ret = RENDER_OK;
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	for (int i = 0; i < m_curr_num; ++i) {
		Sprite* child = m_slots[m_curr[i]];
		rp_child->actor = child->QueryActor(rp.actor);
		ret |= DrawNode::Draw(child, *rp_child);
	}
	RenderParamsPool::Instance()->Push(rp_child); 
	return ret;
}

void AnimTreeCurr::Clear()
{
	m_ctrl.Clear();
	m_curr_num = 0;
}

sm::rect AnimTreeCurr::CalcAABB(const Actor* actor) const
{
	return AABBHelper::CalcAABB(m_slots, actor);
}

void AnimTreeCurr::SetAnimCopy(const AnimCopy* copy)
{
	if (m_copy == copy) {
		return;
	}

	m_copy = copy;

	delete[] m_curr;
	m_curr = new int[copy->m_max_item_num];
	m_curr_num = 0;

	ResetLayerCursor();

	m_slots.resize(m_copy->m_slots.size());
	for (int i = 0, n = m_copy->m_slots.size(); i < n; ++i) 
	{
		Sprite* src = const_cast<Sprite*>(m_copy->m_slots[i]);
		Sprite* dst = VI_CLONE(Sprite, src);
		m_slots[i] = dst;
	}
}

void AnimTreeCurr::LoadSprLerpData(Sprite* spr, const AnimCopy::Lerp& lerp, int time)
{
	SprSRT srt;
	for (int i = 0; i < SprSRT::SRT_MAX; ++i) {
		srt.srt[i] = lerp.srt.srt[i] + lerp.dsrt.srt[i] * time;
	}
	srt.UpdateCenter();
	spr->SetLocalSRT(srt);

	Color mul(lerp.col_mul), add(lerp.col_add);
	mul.r += lerp.dcol_mul[0] * time;
	mul.g += lerp.dcol_mul[1] * time;
	mul.b += lerp.dcol_mul[2] * time;
	mul.a += lerp.dcol_mul[3] * time;
	add.r += lerp.dcol_add[0] * time;
	add.g += lerp.dcol_add[1] * time;
	add.b += lerp.dcol_add[2] * time;
	add.a += lerp.dcol_add[3] * time;

	RenderColor col = spr->GetColor();
	col.SetMul(mul);
	col.SetAdd(add);
	spr->SetColor(col);
}

int AnimTreeCurr::UpdateFrameCursor(bool loop, float interval, int fps, bool reset_cursor)
{
	int curr_frame = (m_ctrl.GetCurrTime() - m_ctrl.GetStartTime()) * fps;
	int max_frame = m_copy->m_max_frame_idx - 1;
	int loop_max_frame = max_frame + interval * fps;
	if (loop) {
		if (curr_frame <= max_frame) {
			;
		} else if (curr_frame > max_frame && curr_frame <= loop_max_frame) {
			curr_frame = 0;
			m_ctrl.SetFrame(0);
			if (reset_cursor) {
				ResetLayerCursor();
			}
		} else {
			curr_frame = 0;
			m_ctrl.SetFrame(0);
			m_ctrl.SetStartTime(m_ctrl.GetCurrTime());
			if (reset_cursor) {
				ResetLayerCursor();
			}
		}
	} else {
		if (curr_frame > max_frame) {
			curr_frame = max_frame;
		}
	}
	return curr_frame;
}

void AnimTreeCurr::ResetLayerCursor()
{
	if (m_copy) {
		m_layer_cursor.assign(m_copy->m_layers.size(), 0);
		m_layer_cursor_update.assign(m_copy->m_layers.size(), false);
	}
}

void AnimTreeCurr::LoadCurrSprites(const UpdateParams& up, const Sprite* spr)
{
	if (m_copy->m_max_item_num < 0) {
		return;
	}

	UpdateCursor();
	LoadCurrSpritesImpl(up, spr);
}

void AnimTreeCurr::UpdateCursor()
{
	int frame = m_ctrl.GetFrame();
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
		m_layer_cursor_update[i] = false;

		const AnimCopy::Layer& layer = m_copy->m_layers[i];
		if (layer.frames.empty()) {
			continue;
		}

		int cursor = m_layer_cursor[i];
		if (cursor == INT_MAX) {
			continue;
		}

		int frame_num = layer.frames.size();
		assert(cursor < frame_num);
		if (cursor >= 0 && cursor < layer.frames.size() && layer.frames[cursor].time == frame + 1) {
			m_layer_cursor_update[i] = true;
		} else {
			while (frame_num > 1 && cursor < frame_num - 1 && layer.frames[cursor + 1].time <= frame + 1) {
				++cursor;
				m_layer_cursor_update[i] = true;
			}
		}
		if (cursor == 0 && frame + 1 < layer.frames[cursor].time) {
			cursor = -1;
		}
		if (cursor == frame_num - 1 && frame + 1 > layer.frames[cursor].time) {
			cursor = INT_MAX;
		}
		m_layer_cursor[i] = cursor;
	}
}

void AnimTreeCurr::LoadCurrSpritesImpl(const UpdateParams& up, const Sprite* spr)
{
	std::vector<std::pair<AnimLerp::SprData, ILerp*> > todo;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);

	int ctrl_frame = m_ctrl.GetFrame();

	m_curr_num = 0;
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
		int cursor = m_layer_cursor[i];
		if (cursor == -1 || cursor == INT_MAX) {
			continue;
		}
		const AnimCopy::Frame& frame = m_copy->m_layers[i].frames[cursor];
		for (int j = 0, m = frame.items.size(); j < m; ++j)
		{
			const AnimCopy::Item& actor = frame.items[j];
			m_curr[m_curr_num++] = actor.slot;
			if (actor.next != -1) 
			{
				assert(actor.lerp != -1);
				const AnimCopy::Frame& next_frame = m_copy->m_layers[i].frames[cursor + 1];

				if (actor.slot != next_frame.items[actor.next].slot) {
					int sym_id = 0;
					if (spr) {
						sym_id = spr->GetSymbol()->GetID();
					}
//					fault("anim lerp err: sym_id %d, frame %d\n", sym_id, frame);
					return;
				}

				assert(actor.slot == next_frame.items[actor.next].slot);
				Sprite* tween = m_slots[actor.slot];
				int time = ctrl_frame + 1 - frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				float process = (float) (ctrl_frame + 1 - frame.time) / (next_frame.time - frame.time);
				const Sprite* begin = actor.spr;
				const Sprite* end = next_frame.items[actor.next].spr;
				AnimLerp::LerpSpecial(begin, end, tween, process);
			}
			else if (actor.prev != -1)
			{
				assert(actor.lerp == -1);
				const AnimCopy::Frame& pre_frame = m_copy->m_layers[i].frames[cursor - 1];
				const AnimCopy::Item& pre_actor = pre_frame.items[actor.prev];
				assert(actor.slot == pre_actor.slot);
				Sprite* tween = m_slots[pre_actor.slot];
				int time = frame.time - pre_frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[pre_actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				AnimLerp::LerpSpecial(pre_actor.spr, actor.spr, tween, 1);
			}
			else
			{
				SprSRT srt;
				m_copy->m_slots[actor.slot]->GetLocalSRT(srt);
				m_slots[actor.slot]->SetLocalSRT(srt);
			}

			bool last_frame = cursor == m_copy->m_layers[i].frames.size() - 1;
			if (!last_frame && m_layer_cursor_update[i] && actor.prev == -1) 
			{
				Sprite* child = m_slots[actor.slot];
				up_child->SetActor(child->QueryActor(up.GetActor()));
				child->OnMessage(*up_child, MSG_TRIGGER);
			}
		}
	}

	UpdateParamsPool::Instance()->Push(up_child); 
}

bool AnimTreeCurr::UpdateChildren(const UpdateParams& up, const Sprite* spr)
{
	bool dirty = false;
	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);
	for (int i = 0; i < m_curr_num; ++i) 
	{
		Sprite* child = m_slots[m_curr[i]];
		up_child->SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(*up_child)) {
			dirty = true;
		}
	}
	UpdateParamsPool::Instance()->Push(up_child); 
	return dirty;
}

void AnimTreeCurr::SetChildrenFrame(const UpdateParams& up, const Sprite* spr, int static_frame, int fps)
{
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
		int cursor = m_layer_cursor[i];
		if (cursor == -1 || cursor == INT_MAX) {
			continue;
		}
		const AnimCopy::Layer& layer = m_copy->m_layers[i];
		const AnimCopy::Frame& frame = layer.frames[cursor];
		for (int j = 0, m = frame.items.size(); j < m; ++j)
		{
			// find first time
			int first_time = frame.time;
			int frame_idx = cursor, actor_idx = j;
			while (frame_idx >= 0 && layer.frames[frame_idx].items[actor_idx].prev != -1) {
				actor_idx = layer.frames[frame_idx].items[actor_idx].prev;
				--frame_idx;
				first_time = layer.frames[frame_idx].time;
			}

			const AnimCopy::Item& actor = frame.items[j];
			Sprite* child = m_slots[actor.slot];

 			SetStaticFrameVisitor visitor(static_frame - first_time + 1);
			SprVisitorParams vp;
			vp.actor = child->QueryActor(up.GetActor());
			child->Traverse(visitor, vp, false);
		}
	}
}

}