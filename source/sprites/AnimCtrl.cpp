#include "AnimCtrl.h"
#include "Animation.h"

#include <assert.h>

namespace s2
{

AnimCtrl::AnimCtrl()
	: m_frame(-1)
	, m_start_time(0)
	, m_curr_time(0)
	, m_stop_time(0)
	, m_stop_during(0)
	, m_active(true)
{
}

void AnimCtrl::SetActive(bool active)
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

void AnimCtrl::SetFrame(int frame)
{
	m_frame = frame;
}

void AnimCtrl::SetFrame(int frame, int fps)
{
	m_frame = frame;

	m_curr_time = Animation::Instance()->GetTime();
	m_start_time = m_curr_time - static_cast<float>(m_frame) / fps;

	m_stop_time = 0;
	m_stop_during = 0;
}

void AnimCtrl::SetTime(float time)
{
	static const float dt = 0.01f;
	m_curr_time = Animation::Instance()->GetTime() - dt;
	m_start_time = m_curr_time - time;
	m_stop_time = 0;
	m_stop_during = -dt;
}

bool AnimCtrl::Update()
{
	float curr_time = Animation::Instance()->GetTime() - m_stop_during;
	assert(m_curr_time <= curr_time);
	if (curr_time == m_curr_time) {
		m_curr_time = curr_time;
		return false;
	} else {
		m_curr_time = curr_time;
		return true;
	}
}

void AnimCtrl::Clear()
{
	Reset();
	m_frame = -1;
}

void AnimCtrl::Reset()
{
	m_start_time = m_curr_time = Animation::Instance()->GetTime();
	m_stop_time = m_stop_during = 0;
}

}