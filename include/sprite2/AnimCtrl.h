#ifndef _SPRITE2_ANIM_CTRL_H_
#define _SPRITE2_ANIM_CTRL_H_

namespace s2
{

class AnimCtrl
{
public:
	AnimCtrl();

	bool IsActive() const { return m_active; }
	void SetActive(bool active);

	int GetFrame() const { return m_frame; }
	void SetFrame(int frame);
	void SetFrame(int frame, int fps);

	void SetTime(float time);

	bool Update();

	void Clear();

	void Reset();

	float GetStartTime() const { return m_start_time; }
	void  SetStartTime(float time) { m_start_time = time; }
	float GetCurrTime() const { return m_curr_time; }

private:
	int m_frame;

	float m_start_time, m_curr_time;
	float m_stop_time, m_stop_during;

	bool m_active;	

}; // AnimCtrl

}

#endif // _SPRITE2_ANIM_CTRL_H_