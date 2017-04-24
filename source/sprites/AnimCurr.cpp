#include "AnimCurr.h"
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
	, m_frame(0)
	, m_start_time(0)
	, m_curr_time(0)
	, m_stop_time(0)
	, m_stop_during(0)
	, m_active(true)
{
	ResetLayerCursor();
}

AnimCurr::AnimCurr(const AnimCurr& curr)
	: m_copy(curr.m_copy)
	, m_layer_cursor(curr.m_layer_cursor)
	, m_layer_cursor_update(curr.m_layer_cursor_update)
	, m_slots(curr.m_slots)
	, m_slotmap(curr.m_slotmap)
	, m_curr_num(curr.m_curr_num)
	, m_frame(curr.m_frame)
	, m_start_time(curr.m_start_time)
	, m_curr_time(curr.m_curr_time)
	, m_stop_time(curr.m_stop_time)
	, m_stop_during(curr.m_stop_during)
	, m_active(curr.m_active)
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
	m_slotmap = curr.m_slotmap;
	m_curr_num = curr.m_curr_num;
	m_curr = new int[curr.m_copy->m_max_item_num];
	memcpy(m_curr, curr.m_curr, curr.m_curr_num * sizeof(int));
	m_frame = curr.m_frame;
	m_start_time = curr.m_start_time;
	m_curr_time = curr.m_curr_time;
	m_stop_time = curr.m_stop_time;
	m_stop_during = curr.m_stop_during;
	m_active = curr.m_active;
	return *this;
}

AnimCurr::~AnimCurr()
{
	delete m_curr;
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

VisitResult AnimCurr::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const
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
			Sprite* actor_holder = m_slots[m_slotmap[m_curr[i]]];
			cp.actor = actor_holder->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	} else {
		for (int i = m_curr_num - 1; i >= 0; --i) 
		{
			Sprite* child = m_slots[m_curr[i]];
			Sprite* actor_holder = m_slots[m_slotmap[m_curr[i]]];
			cp.actor = actor_holder->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	}
	return ret;
}

void AnimCurr::OnMessage(const UpdateParams& up, const Sprite* spr, Message msg)
{
	UpdateParams up_child(up);
	up_child.Push(spr);
	for (int i = 0; i < m_curr_num; ++i) 
	{
		Sprite* child = m_slots[m_curr[i]];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		child->OnMessage(up_child, msg);
	}
}

bool AnimCurr::Update(const UpdateParams& up, const Sprite* spr, 
					  bool loop, float interval, int fps)
{
	bool dirty = false;

	if (!m_active) {
		return dirty;
	}

	// update time
	float curr_time = Animation::Instance()->GetTime() - m_stop_during;
	assert(m_curr_time <= curr_time);
	if (curr_time == m_curr_time) {
		m_curr_time = curr_time;
		return dirty;
	} else {
		m_curr_time = curr_time;
	}

	// update frame
	int curr_frame = (m_curr_time - m_start_time) * fps + 1;
	int max_frame = m_copy->m_max_frame_idx;
	int loop_max_frame = max_frame + interval * fps;
	if (loop) {
		if (curr_frame <= max_frame) {
			;
		} else if (curr_frame > max_frame && curr_frame <= loop_max_frame) {
			curr_frame = 1;
			m_frame = 0;
			ResetLayerCursor();
		} else {
			curr_frame = 1;
			m_frame = 0;
			m_start_time = m_curr_time;
			ResetLayerCursor();
		}
	} else {
		if (curr_frame > max_frame) {
			curr_frame = max_frame;
		}
	}

	// update curr frame
	if (curr_frame != m_frame) 
	{
		m_frame = curr_frame;
		dirty = true;
		LoadCurrSprites(up, spr);
	}

	if (UpdateChildren(up, spr)) {
		dirty = true;
	}

	return dirty;
}

void AnimCurr::Draw(const RenderParams& rp) const
{
	RenderParams rp_child(rp);
	for (int i = 0; i < m_curr_num; ++i) {
		Sprite* child = m_slots[m_curr[i]];
		Sprite* actor_holder = m_slots[m_slotmap[m_curr[i]]];
		rp_child.actor = actor_holder->QueryActor(rp.actor);
		DrawNode::Draw(child, rp_child);
	}
}

Sprite* AnimCurr::FetchChild(const std::string& name) const
{
	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		Sprite* spr = m_slots[i];
		if (spr->GetName() == name) {
			return spr;
		}
	}
	return NULL;
}

Sprite* AnimCurr::FetchChild(int idx) const
{
	if (idx < 0 || idx >= m_slots.size()) {
		return NULL;
	} else {
		return m_slots[idx];
	}
}

void AnimCurr::SetChildAction(const Actor* parent, int symid, const char* action)
{
	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		Sprite* spr = m_slots[i];
		if (spr->GetSymbol()->GetID() == symid) {
			Actor * actor = ActorFactory::Instance()->Create(parent, spr);
			if(actor->GetSpr()->GetSymbol()->Type() == SYM_COMPLEX) {
				const ComplexSymbol* sym_complex = VI_DOWNCASTING<const ComplexSymbol*>(actor->GetSpr()->GetSymbol());
				ComplexActor* actor_complex = static_cast<ComplexActor*>(actor);
				int action_idx = -1;
				if (action) {
					action_idx = sym_complex->GetActionIdx(action);
				}
				actor_complex->SetAction(action_idx);
			}
		}
	}
}

sm::rect AnimCurr::CalcAABB(const Actor* actor) const
{
	sm::rect aabb;
	for (int i = 0; i < m_curr_num; ++i) 
	{
		Sprite* c_spr = m_slots[m_curr[i]];
		const Actor* c_actor = c_spr->QueryActor(actor);
		sm::rect c_aabb = c_spr->GetSymbol()->GetBounding(c_spr, c_actor);
		if (!c_aabb.IsValid()) {
			continue;
		}
		S2_MAT mat = c_spr->GetLocalMat();
		if (c_actor) {
			mat = c_actor->GetLocalMat() * mat;
		}

		aabb.Combine(mat * sm::vec2(c_aabb.xmin, c_aabb.ymin));
		aabb.Combine(mat * sm::vec2(c_aabb.xmax, c_aabb.ymin));
		aabb.Combine(mat * sm::vec2(c_aabb.xmax, c_aabb.ymax));
		aabb.Combine(mat * sm::vec2(c_aabb.xmin, c_aabb.ymax));
	}
	return aabb;
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

void AnimCurr::Start(const UpdateParams& up, const Sprite* spr)
{
	ResetTime();
	ResetLayerCursor();
	m_frame = 1;
	LoadCurrSprites(up, spr);
}

void AnimCurr::SetTime(float time)
{
	static const float dt = 0.01f;
	m_curr_time = Animation::Instance()->GetTime() - dt;
	m_start_time = m_curr_time - time;
	m_stop_time = 0;
	m_stop_during = -dt;
}

void AnimCurr::SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps)
{
	frame = frame % m_copy->m_max_frame_idx + 1;

	if (frame < m_frame) {
		ResetLayerCursor();
	}

	m_frame = frame;

	m_curr_time = Animation::Instance()->GetTime();
	m_start_time = m_curr_time - static_cast<float>(m_frame) / fps;

	m_stop_time = 0;
	m_stop_during = 0;

	LoadCurrSprites(up, spr);

	UpdateChildren(up, spr);
}

void AnimCurr::SetAnimCopy(const AnimCopy* copy)
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

	std::map<std::string,int> name2index;
	m_slotmap.resize(m_slots.size());
	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		Sprite* spr = m_slots[i];
		std::string name = spr->GetName();
		std::map<std::string,int>::iterator iter = name2index.find(name);
		if (iter == name2index.end()) {
			name2index[name] = i;
			m_slotmap[i] = i;
		} else {
			m_slotmap[i] = iter->second;
		}
	}
}

void AnimCurr::SetActive(bool active)
{
	if (m_active == active) {
		return;
	}

	if (active) {
		m_stop_during += Animation::Instance()->GetTime() - m_stop_time;
		m_stop_time = 0;
	} else {
		m_stop_time = Animation::Instance()->GetTime();
	}
	m_active = active; 
}

void AnimCurr::Clear()
{
	ResetTime();
	m_frame = 1;
	m_curr_num = 0;
}

void AnimCurr::ResetTime()
{
	m_start_time = m_curr_time = Animation::Instance()->GetTime();
	m_stop_time = m_stop_during = 0;	
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
		if (cursor >= 0 && cursor < layer.frames.size() && layer.frames[cursor].time == m_frame) {
			m_layer_cursor_update[i] = true;
		} else {
			while (frame_num > 1 && cursor < frame_num - 1 && layer.frames[cursor + 1].time <= m_frame) {
				++cursor;
				m_layer_cursor_update[i] = true;
			}
		}
		if (cursor == 0 && m_frame < layer.frames[cursor].time) {
			cursor = -1;
		}
		if (cursor == frame_num - 1 && m_frame > layer.frames[cursor].time) {
			cursor = INT_MAX;
		}
		m_layer_cursor[i] = cursor;
	}
}

void AnimCurr::LoadCurrSpritesImpl(const UpdateParams& up, const Sprite* spr)
{
	std::vector<std::pair<AnimLerp::SprData, ILerp*> > todo;

	UpdateParams up_child(up);
	up_child.Push(spr);

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
//					fault("anim lerp err: sym_id %d, frame %d\n", sym_id, m_frame);
					return;
				}

				assert(actor.slot == next_frame.items[actor.next].slot);
				Sprite* tween = m_slots[actor.slot];
				int time = m_frame - frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				float process = (float) (m_frame - frame.time) / (next_frame.time - frame.time);
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
				up_child.SetActor(child->QueryActor(up.GetActor()));
				child->OnMessage(up_child, MSG_TRIGGER);
			}
		}
	}
}

bool AnimCurr::UpdateChildren(const UpdateParams& up, const Sprite* spr)
{
	bool dirty = false;
	UpdateParams up_child(up);
	up_child.Push(spr);
	for (int i = 0; i < m_curr_num; ++i) 
	{
		Sprite* child = m_slots[m_curr[i]];
		if (child->IsInheritUpdate()) {
			up_child.SetActor(child->QueryActor(up.GetActor()));
			if (child->Update(up_child)) {
				dirty = true;
			}
		}
	}
	return dirty;
}

}