#include "AnimCurr.h"
#include "AnimCopy.h"
#include "S2_Sprite.h"
#include "Animation.h"
#include "SpriteVisitor.h"
#include "DrawNode.h"
#include "AnimLerp.h"
#include "AnimLerp.h"

#include <algorithm>
#include <climits>

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
	, m_slots(curr.m_slots)
	, m_curr_num(curr.m_curr_num)
	, m_frame(curr.m_frame)
	, m_start_time(curr.m_start_time)
	, m_curr_time(curr.m_curr_time)
	, m_stop_time(curr.m_stop_time)
	, m_stop_during(curr.m_stop_during)
	, m_active(curr.m_active)
{
	m_curr = new int[curr.m_copy->m_max_actor_num];
	memcpy(m_curr, curr.m_curr, curr.m_curr_num * sizeof(int));

	for_each(m_slots.begin(), m_slots.end(), cu::AddRefFunctor<Sprite>());	
}

AnimCurr& AnimCurr::operator = (const AnimCurr& curr)
{	
	m_copy = curr.m_copy;
	m_layer_cursor = curr.m_layer_cursor;
	m_slots = curr.m_slots;
	for_each(m_slots.begin(), m_slots.end(), cu::AddRefFunctor<Sprite>());
	m_curr_num = curr.m_curr_num;
	m_curr = new int[curr.m_copy->m_max_actor_num];
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

	if (visitor.GetOrder()) {
		for (int i = 0; i < m_curr_num; ++i) {
			if (!SpriteVisitor::VisitChild(visitor, params, m_slots[m_curr[i]], ret)) {
				break;
			}
		}
	} else {
		for (int i = m_curr_num - 1; i >= 0; --i) {
			if (!SpriteVisitor::VisitChild(visitor, params, m_slots[m_curr[i]], ret)) {
				break;
			}
		}
	}
	return ret;
}

void AnimCurr::OnMessage(Message msg, const SprTreePath& path)
{
	for (int i = 0; i < m_curr_num; ++i) {
		m_slots[m_curr[i]]->OnMessage(msg, path);
	}
}

bool AnimCurr::Update(const RenderParams& rp, bool loop, float interval, int fps)
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

	// update children
	for (int i = 0; i < m_curr_num; ++i) {
		if (m_slots[m_curr[i]]->Update(rp)) {
			dirty = true;
		}
	}

	// update curr frame
	if (curr_frame != m_frame) {
		m_frame = curr_frame;
		dirty = true;
		LoadCurrSprites(rp.path);
	}

	return dirty;
}

void AnimCurr::Draw(const RenderParams& rp) const
{
	for (int i = 0; i < m_curr_num; ++i) {
		DrawNode::Draw(m_slots[m_curr[i]], rp);
	}
}

Sprite* AnimCurr::FetchChild(const std::string& name, const SprTreePath& path) const
{
// 	for (int i = 0; i < m_curr_num; ++i) {
// 		if (m_slots[m_curr[i]]->GetName() == name) {
// 			return m_slots[m_curr[i]];
// 		}
// 	}

	//////////////////////////////////////////////////////////////////////////

	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		Sprite* spr = m_slots[i];
		if (spr->GetName() == name) {
			return spr;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	return NULL;
}

void AnimCurr::Start(const SprTreePath& path)
{
	ResetTime();
	ResetLayerCursor();
	m_frame = 1;
	LoadCurrSprites(path);
}

void AnimCurr::SetTime(float time)
{
	static const float dt = 0.01f;
	m_curr_time = Animation::Instance()->GetTime() - dt;
	m_start_time = m_curr_time - time;
	m_stop_time = 0;
	m_stop_during = -dt;
}

void AnimCurr::SetFrame(int frame, int fps, const SprTreePath& path)
{
	frame = frame % m_copy->m_max_frame_idx;

	if (frame < m_frame) {
		ResetLayerCursor();
	}

	m_frame = frame;

	m_curr_time = Animation::Instance()->GetTime();
	m_start_time = m_curr_time - static_cast<float>(m_frame) / fps;

	m_stop_time = 0;
	m_stop_during = 0;

	LoadCurrSprites(path);

	// update children
	RenderParams rp;
	for (int i = 0; i < m_curr_num; ++i) {
		m_slots[m_curr[i]]->Update(rp);
	}
}

void AnimCurr::SetAnimCopy(const AnimCopy* copy)
{
	if (m_copy == copy) {
		return;
	}

	m_copy = copy;

	delete[] m_curr;
	m_curr = new int[copy->m_max_actor_num];
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
	}
}

void AnimCurr::LoadCurrSprites(const SprTreePath& path)
{
	if (m_copy->m_max_actor_num < 0) {
		return;
	}

	bool cursor_update = m_frame == 1;
	UpdateCursor(cursor_update);
	LoadCurrSprites(cursor_update, path);
}

void AnimCurr::UpdateCursor(bool cursor_update)
{
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
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
		while (frame_num > 1 && cursor < frame_num - 1 && layer.frames[cursor + 1].time <= m_frame) {
			++cursor;
			cursor_update = true;
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

void AnimCurr::LoadCurrSprites(bool cursor_update, const SprTreePath& path)
{
	std::vector<std::pair<AnimLerp::SprData, ILerp*> > todo;

	m_curr_num = 0;
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
		int cursor = m_layer_cursor[i];
		if (cursor == -1 || cursor == INT_MAX) {
			continue;
		}
		const AnimCopy::Frame& frame = m_copy->m_layers[i].frames[cursor];
		for (int j = 0, m = frame.actors.size(); j < m; ++j)
		{
			const AnimCopy::Actor& actor = frame.actors[j];
			m_curr[m_curr_num++] = actor.slot;
			if (actor.next != -1) 
			{
				assert(actor.lerp != -1);
				const AnimCopy::Frame& next_frame = m_copy->m_layers[i].frames[cursor + 1];

				assert(actor.slot == next_frame.actors[actor.next].slot);
				Sprite* tween = m_slots[actor.slot];
				int time = m_frame - frame.time;
				const AnimCopy::Lerp& lerp = m_copy->m_lerps[actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				float process = (float) (m_frame - frame.time) / (next_frame.time - frame.time);
				const Sprite* begin = actor.spr;
				const Sprite* end = next_frame.actors[actor.next].spr;
				AnimLerp::LerpSpecial(begin, end, tween, process);
			}
			else if (actor.prev != -1)
			{
				assert(actor.lerp == -1);
				const AnimCopy::Frame& pre_frame = m_copy->m_layers[i].frames[cursor - 1];
				const AnimCopy::Actor& pre_actor = pre_frame.actors[actor.prev];
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

			if (cursor_update && actor.prev == -1) {
				Sprite* spr = m_slots[actor.slot];
				m_slots[actor.slot]->OnMessage(MSG_TRIGGER, path);
			}
		}
	}
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

}