#include "AnimCurr3.h"
#include "AnimCopy2.h"
#include "S2_Sprite.h"
#include "Animation.h"
#include "SprVisitor.h"
#include "DrawNode.h"
#include "AnimLerp.h"
#include "AnimLerp.h"

#include <algorithm>

namespace s2
{

AnimCurr3::AnimCurr3()
	: m_copy(NULL)
	, m_frame(0)
	, m_start_time(0)
	, m_curr_time(0)
	, m_stop_time(0)
	, m_stop_during(0)
	, m_active(true)
{
	ResetLayerCursor();
}

AnimCurr3::AnimCurr3(const AnimCurr3& curr)
	: m_copy(curr.m_copy)
	, m_layer_cursor(curr.m_layer_cursor)
	, m_slots(curr.m_slots)
	, m_curr(curr.m_curr)
	, m_frame(curr.m_frame)
	, m_start_time(curr.m_start_time)
	, m_curr_time(curr.m_curr_time)
	, m_stop_time(curr.m_stop_time)
	, m_stop_during(curr.m_stop_during)
	, m_active(curr.m_active)
{
	for_each(m_slots.begin(), m_slots.end(), cu::AddRefFunctor<Sprite>());	
}

AnimCurr3& AnimCurr3::operator = (const AnimCurr3& curr)
{	
	m_copy = curr.m_copy;
	m_layer_cursor = curr.m_layer_cursor;
	m_slots = curr.m_slots;
	m_curr = curr.m_curr;
	m_frame = curr.m_frame;
	m_start_time = curr.m_start_time;
	m_curr_time = curr.m_curr_time;
	m_stop_time = curr.m_stop_time;
	m_stop_during = curr.m_stop_during;
	m_active = curr.m_active;
	return *this;
}

AnimCurr3::~AnimCurr3()
{
	for_each(m_slots.begin(), m_slots.end(), cu::RemoveRefFunctor<Sprite>());
}

bool AnimCurr3::Traverse(SprVisitor& visitor, const SprVisitorParams& params) const
{
	if (visitor.GetOrder()) {
		for (int i = 0, n = m_curr.size(); i < n; ++i) {
			if (!m_slots[m_curr[i]]->Traverse(visitor, params)) {
				return false;
			}
		}
	} else {
		for (int i = m_curr.size() - 1; i >= 0; --i) {
			if (!m_slots[m_curr[i]]->Traverse(visitor, params)) {
				return false;
			}
		}
	}
	return true;
}

void AnimCurr3::OnMessage(Message msg)
{
	for (int i = 0, n = m_curr.size(); i < n; ++i) {
		m_slots[m_curr[i]]->OnMessage(msg);
	}
}

bool AnimCurr3::Update(const RenderParams& params, bool loop, float interval, int fps)
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
	for (int i = 0, n = m_curr.size(); i < n; ++i) {
		if (m_slots[m_curr[i]]->Update(params)) {
			dirty = true;
		}
	}

	// update curr frame
	if (curr_frame != m_frame) {
		m_frame = curr_frame;
		dirty = true;
		LoadCurrSprites();
	}

	return dirty;
}

void AnimCurr3::Draw(const RenderParams& params) const
{
	for (int i = 0, n = m_curr.size(); i < n; ++i) {
		DrawNode::Draw(m_slots[m_curr[i]], params);
	}
}

Sprite* AnimCurr3::FetchChild(const std::string& name) const
{
	for (int i = 0, n = m_curr.size(); i < n; ++i) {
		if (m_slots[m_curr[i]]->GetName() == name) {
			return m_slots[m_curr[i]];
		}
	}
	return NULL;
}

void AnimCurr3::Start()
{
	ResetTime();
	ResetLayerCursor();
	m_frame = 1;
	LoadCurrSprites();
	OnMessage(MSG_START);
}

void AnimCurr3::SetTime(float time)
{
	static const float dt = 0.01f;
	m_curr_time = Animation::Instance()->GetTime() - dt;
	m_start_time = m_curr_time - time;
	m_stop_time = 0;
	m_stop_during = -dt;
}

void AnimCurr3::SetFrame(int frame, int fps)
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

	LoadCurrSprites();

	// update children
	RenderParams params;
	for (int i = 0, n = m_curr.size(); i < n; ++i) {
		m_slots[m_curr[i]]->Update(params);
	}
}

void AnimCurr3::SetAnimCopy(const AnimCopy2* copy)
{
	if (m_copy == copy) {
		return;
	}

	m_copy = copy;

	ResetLayerCursor();

	m_slots.resize(m_copy->m_slots.size());
	for (int i = 0, n = m_copy->m_slots.size(); i < n; ++i) 
	{
		Sprite* src = const_cast<Sprite*>(m_copy->m_slots[i]);
		Sprite* dst = VI_CLONE(Sprite, src);
		m_slots[i] = dst;
	}
}

void AnimCurr3::SetActive(bool active)
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

void AnimCurr3::Clear()
{
	ResetTime();
	m_frame = 1;
	m_curr.clear();
}

void AnimCurr3::ResetTime()
{
	m_start_time = m_curr_time = Animation::Instance()->GetTime();
	m_stop_time = m_stop_during = 0;	
}

void AnimCurr3::ResetLayerCursor()
{
	if (m_copy) {
		m_layer_cursor.assign(m_copy->m_layers.size(), 0);
	}
}

void AnimCurr3::LoadCurrSprites()
{
	if (m_copy->m_max_actor_num < 0) {
		return;
	}

	// update cursor
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
		const AnimCopy2::Layer& layer = m_copy->m_layers[i];
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
		}
		if (cursor == 0 && m_frame < layer.frames[cursor].time) {
			cursor = -1;
		}
		if (cursor == frame_num - 1 && m_frame > layer.frames[cursor].time) {
			cursor = INT_MAX;
		}
		m_layer_cursor[i] = cursor;
	}

	// load sprites
	std::vector<std::pair<AnimLerp::SprData, ILerp*> > todo;

	m_curr.clear();
	m_curr.reserve(m_copy->m_max_actor_num);
	for (int i = 0, n = m_layer_cursor.size(); i < n; ++i)
	{
		int cursor = m_layer_cursor[i];
		if (cursor == -1 || cursor == INT_MAX) {
			continue;
		}
		const AnimCopy2::Frame& frame = m_copy->m_layers[i].frames[cursor];
		for (int j = 0, m = frame.actors.size(); j < m; ++j)
		{
			const AnimCopy2::Actor& actor = frame.actors[j];
			m_curr.push_back(actor.slot);
			if (actor.next != -1) 
			{
				assert(actor.lerp != -1);
				const AnimCopy2::Frame& next_frame = m_copy->m_layers[i].frames[cursor + 1];
				assert(actor.slot == next_frame.actors[actor.next].slot);
				Sprite* tween = m_slots[actor.slot];
				int time = m_frame - frame.time;
				const AnimCopy2::Lerp& lerp = m_copy->m_lerps[actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				float process = (float) (m_frame - frame.time) / (next_frame.time - frame.time);
				const Sprite* begin = actor.spr;
				const Sprite* end = next_frame.actors[actor.next].spr;
				AnimLerp::LerpSpecial(begin, end, tween, process);
			}
			else if (actor.prev != -1)
			{
				assert(actor.lerp == -1);
				const AnimCopy2::Frame& pre_frame = m_copy->m_layers[i].frames[cursor - 1];
				const AnimCopy2::Actor& pre_actor = pre_frame.actors[actor.prev];
				assert(actor.slot == pre_actor.slot);
				Sprite* tween = m_slots[pre_actor.slot];
				int time = frame.time - pre_frame.time;
				const AnimCopy2::Lerp& lerp = m_copy->m_lerps[pre_actor.lerp];
				LoadSprLerpData(tween, lerp, time);

				AnimLerp::LerpSpecial(pre_actor.spr, actor.spr, tween, 1);
			}
			else
			{
				*m_slots[actor.slot] = *m_copy->m_slots[actor.slot];
			}
		}
	}
}

void AnimCurr3::LoadSprLerpData(Sprite* spr, const AnimCopy2::Lerp& lerp, int time)
{
	spr->SetShear(lerp.shear + lerp.dshear * time);
	spr->SetScale(lerp.scale + lerp.dscale * time);
	spr->SetOffset(lerp.offset + lerp.doffset * time);
	spr->SetAngle(lerp.angle + lerp.dangle * time);
	spr->SetPosition(lerp.pos + lerp.dpos * time);

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