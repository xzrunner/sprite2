#include "sprite2/AnimCurr.h"
#include "sprite2/AnimCopy.h"
#include "sprite2/Sprite.h"
#include "sprite2/Animation.h"
#include "sprite2/SpriteVisitor.h"
#include "sprite2/DrawNode.h"
#include "sprite2/AnimLerp.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/Symbol.h"
#include "sprite2/Actor.h"
#include "sprite2/ActorFactory.h"
#include "sprite2/SymType.h"
#include "sprite2/ComplexSymbol.h"
#include "sprite2/ComplexActor.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/ProxyHelper.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/SetStaticFrameVisitor.h"
#include "sprite2/AABBHelper.h"

#include <cu/cu_stl.h>

#include <algorithm>
#include <climits>

#include <assert.h>
#include <string.h>
#include <fault.h>

namespace s2
{

AnimCurr::AnimCurr()
	: m_copy(nullptr)
{
	ResetLayerCursor();
}

AnimCurr::AnimCurr(const AnimCurr& curr)
	: m_copy(curr.m_copy)
	, m_layer_cursor(curr.m_layer_cursor)
	, m_layer_cursor_update(curr.m_layer_cursor_update)
	, m_slots(curr.m_slots)
	, m_curr(curr.m_curr)
	, m_curr_num(curr.m_curr_num)
{
}

AnimCurr& AnimCurr::operator = (const AnimCurr& curr)
{	
	m_copy = curr.m_copy;
	m_layer_cursor = curr.m_layer_cursor;
	m_layer_cursor_update = curr.m_layer_cursor_update;
	m_slots = curr.m_slots;
	m_curr = curr.m_curr;
	m_curr_num = curr.m_curr_num;
	m_ctrl = curr.m_ctrl;
	return *this;
}

//AnimCurr::AnimCurrPtr AnimCurr::Clone() const
//{
//	return std::make_unique<AnimCurr>(*this);
//}

void AnimCurr::AssignSameStruct(const AnimCurr& src)
{
	assert(m_copy == src.m_copy);

	m_ctrl = src.m_ctrl;
	m_layer_cursor = src.m_layer_cursor;
	m_layer_cursor_update = src.m_layer_cursor_update;

	assert(m_slots.size() == src.m_slots.size());
	if (!m_slots.empty())
	{
		const SprPtr* s = &src.m_slots[0];
		SprPtr* d = &m_slots[0];
		SprSRT srt;
		for (int i = 0, n = m_slots.size(); i < n; ++i, ++s, ++d) {
			(*s)->GetLocalSRT(srt);
			(*d)->SetLocalSRT(srt);
			(*d)->SetColor((*s)->GetColor());
		}

		(*s)->SetVisible((*d)->IsVisible());
	}

	m_curr = src.m_curr;
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
	if (m_curr.empty()) {
		return;
	}

	UpdateParams up_child(up);
	up_child.Push(spr);
	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) 
	{
		auto& child = m_slots[*curr];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		child->OnMessage(up_child, msg);
	}
}

SprPtr AnimCurr::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	CU_VEC<std::pair<const ActorConstPtr, SprPtr>> group;
	for (auto& child : m_slots) {
		if (child->GetName() == name) {
			group.push_back(std::make_pair(actor, child));
		}
	}
	return ProxyHelper::BuildGroup(group);
}

SprPtr AnimCurr::FetchChildByIdx(int idx) const
{
	if (idx >= 0 && idx < static_cast<int>(m_slots.size())) {
		return m_slots[idx];
	} else {
		return nullptr;
	}
}

VisitResult AnimCurr::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	if (m_slots.empty()) {
		return ret;
	}

	SprVisitorParams cp = params;
	if (visitor.GetOrder()) 
	{
		const SprPtr* ptr = &m_slots[0];
		for (int i = 0, n = m_slots.size(); i < n; ++i, ++ptr) 
		{
			CU_STR str;
			SprNameMap::Instance()->IDToStr((*ptr)->GetName(), str);			
			cp.actor = (*ptr)->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, *ptr, ret)) {
				break;
			}
		}
	}
	else {
		const SprPtr* ptr = &m_slots[m_slots.size() - 1];
		for (int i = 0, n = m_slots.size(); i < n; ++i, --ptr) {
			CU_STR str;
			SprNameMap::Instance()->IDToStr((*ptr)->GetName(), str);
			cp.actor = (*ptr)->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, *ptr, ret)) {
				break;
			}
		}
	}
	return ret;
}

// todo: all slots, old is m_curr
VisitResult AnimCurr::Traverse2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const
{
	VisitResult ret = VISIT_OVER;
	if (m_curr_num == 0) {
		return ret;
	}

	SprVisitorParams2 cp = params;
	if (visitor.GetOrder()) 
	{
		const SprPtr* ptr = &m_slots[0];
		for (int i = 0, n = m_slots.size(); i < n; ++i, ++ptr) 
		{
			CU_STR str;
			SprNameMap::Instance()->IDToStr((*ptr)->GetName(), str);			
			cp.actor = (*ptr)->QueryActorRef(params.actor.get());
			if (!SpriteVisitor2::VisitChild(visitor, cp, *ptr, ret)) {
				break;
			}
		}
	}
	else {
		const SprPtr* ptr = &m_slots[m_slots.size() - 1];
		for (int i = 0, n = m_slots.size(); i < n; ++i, --ptr) {
			CU_STR str;
			SprNameMap::Instance()->IDToStr((*ptr)->GetName(), str);
			cp.actor = (*ptr)->QueryActorRef(params.actor.get());
			if (!SpriteVisitor2::VisitChild(visitor, cp, *ptr, ret)) {
				break;
			}
		}
	}
	return ret;
}

//VisitResult AnimCurr::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const
//{
//	VisitResult ret = VISIT_OVER;
//	if (m_curr.empty()) {
//		return ret;
//	}
//
//	SprVisitorParams cp = params;
//	if (visitor.GetOrder()) {
//		int* curr = &m_curr[0];
//		for (int i = 0; i < m_curr_num; ++i, ++curr) 
//		{
//			Sprite* child = m_slots[*curr];
//			cp.actor = child->QueryActor(params.actor.get());
//			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
//				break;
//			}
//		}
//	} else {
//		int* curr = &m_curr[m_curr_num - 1];
//		for (int i = m_curr_num - 1; i >= 0; --i, --curr) 
//		{
//			Sprite* child = m_slots[*curr];
//			cp.actor = child->QueryActor(params.actor.get());
//			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
//				break;
//			}
//		}
//	}
//	return ret;
//}

RenderReturn AnimCurr::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	if (m_curr.empty()) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	const int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) {
		const SprPtr& child = m_slots[*curr];
		rp_child->actor = child->QueryActor(rp.actor);
		ret |= DrawNode::Draw(dlist, child.get(), *rp_child);
	}

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
	if (m_copy == copy) {
		return;
	}

	m_copy = copy;

	m_curr.clear();
	m_curr.resize(copy->m_max_item_num);
	m_curr_num = 0;

	ResetLayerCursor();

	m_slots.resize(m_copy->m_slots.size());
	for (int i = 0, n = m_copy->m_slots.size(); i < n; ++i) {
		m_slots[i] = m_copy->m_slots[i]->Clone();
	}

	UpdateSlotsVisible();
}

void AnimCurr::LoadSprLerpData(Sprite& spr, const AnimCopy::Lerp& lerp, int time)
{
	SprSRT srt;
	for (int i = 0; i < SprSRT::SRT_MAX; ++i) {
		srt.srt[i] = lerp.srt.srt[i] + lerp.dsrt.srt[i] * time;
	}
	srt.UpdateCenter();
	spr.SetLocalSRT(srt);

	Color mul(lerp.col_mul), add(lerp.col_add);
	mul.r += static_cast<uint8_t>(lerp.dcol_mul[0] * time);
	mul.g += static_cast<uint8_t>(lerp.dcol_mul[1] * time);
	mul.b += static_cast<uint8_t>(lerp.dcol_mul[2] * time);
	mul.a += static_cast<uint8_t>(lerp.dcol_mul[3] * time);
	add.r += static_cast<uint8_t>(lerp.dcol_add[0] * time);
	add.g += static_cast<uint8_t>(lerp.dcol_add[1] * time);
	add.b += static_cast<uint8_t>(lerp.dcol_add[2] * time);
	add.a += static_cast<uint8_t>(lerp.dcol_add[3] * time);

	RenderColor col = spr.GetColor();
	col.SetMul(mul);
	col.SetAdd(add);
	spr.SetColor(col);
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
		const AnimCopy::Frame* first_frame_ptr = &layer_ptr->frames[0];
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

	UpdateParams up_child(up);
	up_child.Push(spr);

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
		if (frame.items.empty()) {
			continue;
		}
		const AnimCopy::Item* actor_ptr = &frame.items[0];
		for (int i = 0, n = frame.items.size(); i < n; ++i, ++actor_ptr)
		{
			const AnimCopy::Item& actor = *actor_ptr;
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
				SprPtr& tween = m_slots[actor.slot];
				int time = ctrl_frame + 1 - frame.time;
				int tot_time = next_frame.time - frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[actor.lerp];
				LoadSprLerpData(*tween, lerp, time);

				auto& begin = actor.spr;
				auto& end = next_frame.items[actor.next].spr;
				AnimLerp::LerpSpecial(*begin, *end, tween, time, tot_time);

				AnimLerp::LerpExpression(*begin, *end, tween, time, tot_time, frame.lerps);
			}
			else if (actor.prev != -1)
			{
				assert(actor.lerp == -1);
				const AnimCopy::Frame& pre_frame = layer.frames[cursor - 1];
				const AnimCopy::Item& pre_actor = pre_frame.items[actor.prev];
				assert(actor.slot == pre_actor.slot);
				SprPtr& tween = m_slots[pre_actor.slot];
				int time = frame.time - pre_frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[pre_actor.lerp];
				LoadSprLerpData(*tween, lerp, time);

				AnimLerp::LerpSpecial(*pre_actor.spr, *actor.spr, tween, time, time);
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
				SprPtr& child = m_slots[actor.slot];
				up_child.SetActor(child->QueryActor(up.GetActor()));
				child->OnMessage(up_child, MSG_TRIGGER);
			}
		}
	}

	UpdateSlotsVisible();
}

bool AnimCurr::UpdateChildren(const UpdateParams& up, const Sprite* spr)
{
	if (m_curr.empty()) {
		return false;
	}

	bool dirty = false;
	UpdateParams up_child(up);
	up_child.Push(spr);
	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr)
	{
		SprPtr& child = m_slots[*curr];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(up_child)) {
			dirty = true;
		}
	}
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
			SprPtr& child = m_slots[actor.slot];

 			SetStaticFrameVisitor visitor(static_frame - first_time + 1);
			SprVisitorParams vp;
			vp.actor = child->QueryActor(up.GetActor());
			child->Traverse(visitor, vp, false);
		}
	}
}

void AnimCurr::UpdateSlotsVisible()
{
	if (!m_slots.empty()) {
		const SprPtr* slot_ptr = &m_slots[0];
		for (int i = 0, n = m_slots.size(); i < n; ++i, ++slot_ptr) {
			(*slot_ptr)->SetVisible(false);
		}
	}

	if (m_curr.empty()) {
		return;
	}

	int* curr = &m_curr[0];
	for (int i = 0; i < m_curr_num; ++i, ++curr) {
		m_slots[*curr]->SetVisible(true);
	}
}

}