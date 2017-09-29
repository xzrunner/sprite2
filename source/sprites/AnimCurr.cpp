#include "AnimCurr.h"
#include "AnimCopy.h"
#include "S2_Sprite.h"
#include "Animation.h"
#include "SpriteVisitor.h"
#include "DrawNode.h"
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

AnimCurr::AnimCurr()
	: m_copy(NULL)
	, m_curr(NULL)
{
	ResetLayerCursor();
}

AnimCurr::AnimCurr(const AnimCurr& curr)
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

AnimCurr& AnimCurr::operator = (const AnimCurr& curr)
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

AnimCurr::~AnimCurr()
{
	delete m_curr;
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

std::unique_ptr<AnimCurr> AnimCurr::Clone() const
{
	return std::make_unique<AnimCurr>(*this);
}

void AnimCurr::AssignSameStruct(const AnimCurr& src)
{
	assert(m_copy == src.m_copy);

	m_ctrl = src.m_ctrl;
	m_layer_cursor = src.m_layer_cursor;
	m_layer_cursor_update = src.m_layer_cursor_update;

	assert(m_slots.size() == src.m_slots.size());
	if (!m_slots.empty())
	{
		Sprite*const* s = &src.m_slots[0];
		Sprite** d = &m_slots[0];
		SprSRT srt;
		for (int i = 0, n = m_slots.size(); i < n; ++i, ++s, ++d) {
			(*s)->GetLocalSRT(srt);
			(*d)->SetLocalSRT(srt);
			(*d)->SetColor((*s)->GetColor());
		}

		(*s)->SetVisible((*d)->IsVisible());
	}

	assert(m_curr && src.m_curr);
	memcpy(m_curr, src.m_curr, sizeof(int) * m_copy->m_max_item_num);
	m_curr_num = src.m_curr_num;
}

bool AnimCurr::Update(const UpdateParams& up, const Symbol* sym, const Sprite* spr, 
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

bool AnimCurr::SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps)
{
	if (frame == m_ctrl.GetFrame()) {
		return false;
	}

	int frame_copy = frame;
	frame = frame % (m_copy->m_max_frame_idx);

	if (frame < m_ctrl.GetFrame()) {
		ResetLayerCursor();
	}

	m_ctrl.SetFrame(frame, fps);

	LoadCurrSprites(up, spr);

	SetChildrenFrame(up, spr, frame_copy, fps);

	return true;
}

void AnimCurr::Start(const UpdateParams& up, const Sprite* spr)
{
	m_ctrl.Reset();
	m_ctrl.SetFrame(0);

	ResetLayerCursor();
	LoadCurrSprites(up, spr);
}

void AnimCurr::OnMessage(const UpdateParams& up, const Sprite* spr, Message msg)
{
	if (m_curr_num == 0) {
		return;
	}

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);
	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) 
	{
		Sprite* child = m_slots[*curr];
		up_child->SetActor(child->QueryActor(up.GetActor()));
		child->OnMessage(*up_child, msg);
	}
	UpdateParamsPool::Instance()->Push(up_child); 
}

Sprite* AnimCurr::FetchChildByName(int name, const Actor* actor) const
{
	std::vector<std::pair<const Actor*, Sprite*> > group;
	for (auto child : m_slots) {
		if (child->GetName() == name) {
			group.push_back(std::make_pair(actor, child));
		}
	}
	return ProxyHelper::BuildGroup(group);
}

Sprite* AnimCurr::FetchChildByIdx(int idx) const
{
	Sprite* ret = NULL;
	if (idx >= 0 && idx < static_cast<int>(m_slots.size())) {
		ret = m_slots[idx];
	}
	if (ret) {
		ret->AddReference();
	}
	return ret;
}

VisitResult AnimCurr::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	if (m_slots.empty()) {
		return ret;
	}

	SprVisitorParams cp = params;
	if (visitor.GetOrder()) {
		Sprite*const* ptr = &m_slots[0];
		for (int i = 0, n = m_slots.size(); i < n; ++i, ++ptr) {
			std::string str;
			SprNameMap::Instance()->IDToStr((*ptr)->GetName(), str);			
			cp.actor = (*ptr)->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, (*ptr), ret)) {
				break;
			}
		}
	}
	else {
		Sprite*const* ptr = &m_slots[m_slots.size() - 1];
		for (int i = 0, n = m_slots.size(); i < n; ++i, --ptr) {
			std::string str;
			SprNameMap::Instance()->IDToStr((*ptr)->GetName(), str);
			cp.actor = (*ptr)->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, (*ptr), ret)) {
				break;
			}
		}
	}
	return ret;
}

//VisitResult AnimCurr::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const
//{
//	VisitResult ret = VISIT_OVER;
//	if (m_curr_num == 0) {
//		return ret;
//	}
//
//	SprVisitorParams cp = params;
//	if (visitor.GetOrder()) {
//		int* curr = &m_curr[0];
//		for (int i = 0; i < m_curr_num; ++i, ++curr) 
//		{
//			Sprite* child = m_slots[*curr];
//			cp.actor = child->QueryActor(params.actor);
//			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
//				break;
//			}
//		}
//	} else {
//		int* curr = &m_curr[m_curr_num - 1];
//		for (int i = m_curr_num - 1; i >= 0; --i, --curr) 
//		{
//			Sprite* child = m_slots[*curr];
//			cp.actor = child->QueryActor(params.actor);
//			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
//				break;
//			}
//		}
//	}
//	return ret;
//}

RenderReturn AnimCurr::Draw(const RenderParams& rp) const
{
	if (m_curr_num == 0) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) {
		Sprite* child = m_slots[*curr];
		rp_child->actor = child->QueryActor(rp.actor);
		ret |= DrawNode::Draw(child, *rp_child);
	}
	RenderParamsPool::Instance()->Push(rp_child); 
	return ret;
}

void AnimCurr::Clear()
{
	m_ctrl.Clear();
	m_curr_num = 0;
	UpdateSlotsVisible();
}

sm::rect AnimCurr::CalcAABB(const Actor* actor) const
{
	return AABBHelper::CalcAABB(m_slots, actor);
}

void AnimCurr::SetAnimCopy(const std::shared_ptr<AnimCopy>& copy)
{
	if (m_copy.get() == copy.get()) {
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

	UpdateSlotsVisible();
}

void AnimCurr::LoadSprLerpData(Sprite* spr, const AnimCopy::Lerp& lerp, int time)
{
	SprSRT srt;
	for (int i = 0; i < SprSRT::SRT_MAX; ++i) {
		srt.srt[i] = lerp.srt.srt[i] + lerp.dsrt.srt[i] * time;
	}
	srt.UpdateCenter();
	spr->SetLocalSRT(srt);

	Color mul(lerp.col_mul), add(lerp.col_add);
	mul.r += static_cast<uint8_t>(lerp.dcol_mul[0] * time);
	mul.g += static_cast<uint8_t>(lerp.dcol_mul[1] * time);
	mul.b += static_cast<uint8_t>(lerp.dcol_mul[2] * time);
	mul.a += static_cast<uint8_t>(lerp.dcol_mul[3] * time);
	add.r += static_cast<uint8_t>(lerp.dcol_add[0] * time);
	add.g += static_cast<uint8_t>(lerp.dcol_add[1] * time);
	add.b += static_cast<uint8_t>(lerp.dcol_add[2] * time);
	add.a += static_cast<uint8_t>(lerp.dcol_add[3] * time);

	RenderColor col = spr->GetColor();
	col.SetMul(mul);
	col.SetAdd(add);
	spr->SetColor(col);
}

int AnimCurr::UpdateFrameCursor(bool loop, float interval, int fps, bool reset_cursor)
{
	int curr_frame = static_cast<int>((m_ctrl.GetCurrTime() - m_ctrl.GetStartTime()) * fps);
	int max_frame = m_copy->m_max_frame_idx - 1;
	int loop_max_frame = static_cast<int>(max_frame + interval * fps);
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

void AnimCurr::ResetLayerCursor()
{
	if (m_copy) {
		m_layer_cursor.assign(m_copy->m_layers.size(), 0);
		m_layer_cursor_update.assign(m_copy->m_layers.size(), false);
	}
}

void AnimCurr::LoadCurrSprites(const UpdateParams& up, const Sprite* spr)
{
	if (m_copy->m_max_item_num < 0) {
		return;
	}

	UpdateCursor();
	LoadCurrSpritesImpl(up, spr);
}

void AnimCurr::UpdateCursor()
{
	if (m_layer_cursor.empty()) {
		return;
	}

	assert(m_layer_cursor.size() == m_layer_cursor_update.size());
	
	int frame = m_ctrl.GetFrame();
	int* layer_cursor_ptr = &m_layer_cursor[0];
	int* layer_cursor_update_ptr = &m_layer_cursor_update[0];
	const AnimCopy::Layer* layer_ptr = &m_copy->m_layers[0];
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i, ++layer_cursor_ptr, ++layer_cursor_update_ptr, ++layer_ptr)
	{
		*layer_cursor_update_ptr = false;

		const int frame_num = layer_ptr->frames.size();
		if (frame_num == 0) {
			continue;
		}

		int cursor = *layer_cursor_ptr;
		if (cursor == INT_MAX) {
			continue;
		}

		assert(cursor < frame_num && frame_num > 0);
		const s2::AnimCopy::Frame* first_frame_ptr = &layer_ptr->frames[0];
		if (cursor >= 0 && cursor < frame_num && (first_frame_ptr + cursor)->time == frame + 1) {
			*layer_cursor_update_ptr = true;
		} else {
			while (frame_num > 1 && cursor < frame_num - 1 && (first_frame_ptr + cursor + 1)->time <= frame + 1) {
				++cursor;
				*layer_cursor_update_ptr = true;
			}
		}
		if (cursor == 0 && frame + 1 < (first_frame_ptr + cursor)->time) {
			cursor = -1;
		}
		if (cursor == frame_num - 1 && frame + 1 > (first_frame_ptr + cursor)->time) {
			cursor = INT_MAX;
		}
		*layer_cursor_ptr = cursor;
	}
}

void AnimCurr::LoadCurrSpritesImpl(const UpdateParams& up, const Sprite* spr)
{
	if (m_layer_cursor.empty()) {
		return;
	}

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);

	int ctrl_frame = m_ctrl.GetFrame();

	m_curr_num = 0;
	int* layer_cursor_ptr = &m_layer_cursor[0];
	int* layer_cursor_update_ptr = &m_layer_cursor_update[0];
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i, ++layer_cursor_ptr, ++layer_cursor_update_ptr)
	{
		int cursor = *layer_cursor_ptr;
		if (cursor == -1 || cursor == INT_MAX) {
			continue;
		}
		const AnimCopy::Layer& layer = m_copy->m_layers[i];
		const AnimCopy::Frame& frame = layer.frames[cursor];
		for (const auto& actor : frame.items)
		{
			m_curr[m_curr_num++] = actor.slot;
			if (actor.next != -1) 
			{
				assert(actor.lerp != -1);
				const AnimCopy::Frame& next_frame = layer.frames[cursor + 1];

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
				int tot_time = next_frame.time - frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				const Sprite* begin = actor.spr;
				const Sprite* end = next_frame.items[actor.next].spr;
				AnimLerp::LerpSpecial(begin, end, tween, time, tot_time);

				AnimLerp::LerpExpression(begin, end, tween, time, tot_time, frame.lerps);
			}
			else if (actor.prev != -1)
			{
				assert(actor.lerp == -1);
				const AnimCopy::Frame& pre_frame = layer.frames[cursor - 1];
				const AnimCopy::Item& pre_actor = pre_frame.items[actor.prev];
				assert(actor.slot == pre_actor.slot);
				Sprite* tween = m_slots[pre_actor.slot];
				int time = frame.time - pre_frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[pre_actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				AnimLerp::LerpSpecial(pre_actor.spr, actor.spr, tween, time, time);
			}
			else
			{
				SprSRT srt;
				m_copy->m_slots[actor.slot]->GetLocalSRT(srt);
				m_slots[actor.slot]->SetLocalSRT(srt);
			}

			bool last_frame = cursor == layer.frames.size() - 1;
			if (!last_frame && *layer_cursor_update_ptr && actor.prev == -1)
			{
				Sprite* child = m_slots[actor.slot];
				up_child->SetActor(child->QueryActor(up.GetActor()));
				child->OnMessage(*up_child, MSG_TRIGGER);
			}
		}
	}

	UpdateParamsPool::Instance()->Push(up_child); 

	UpdateSlotsVisible();
}

bool AnimCurr::UpdateChildren(const UpdateParams& up, const Sprite* spr)
{
	if (m_curr_num == 0) {
		return false;
	}

	bool dirty = false;
	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);
	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) 
	{
		Sprite* child = m_slots[*curr];
		up_child->SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(*up_child)) {
			dirty = true;
		}
	}
	UpdateParamsPool::Instance()->Push(up_child); 
	return dirty;
}

void AnimCurr::SetChildrenFrame(const UpdateParams& up, const Sprite* spr, int static_frame, int fps)
{
	if (m_layer_cursor.empty()) {
		return;
	}

	int* layer_cursor_ptr = &m_layer_cursor[0];
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i, ++layer_cursor_ptr)
	{
		int cursor = *layer_cursor_ptr;
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

void AnimCurr::UpdateSlotsVisible()
{
	for (auto spr : m_slots) {
		spr->SetVisible(false);
	}

	if (m_curr_num == 0) {
		return;
	}

	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) {
		m_slots[*curr]->SetVisible(true);
	}
}

}