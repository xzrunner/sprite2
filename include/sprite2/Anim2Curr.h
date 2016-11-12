#ifndef _SPRITE2_ANIM2_CURR_H_
#define _SPRITE2_ANIM2_CURR_H_

#include <vector>

#include <stdint.h>

struct rg_skeleton_pose;

namespace s2
{

class Anim2Symbol;

class Anim2Curr
{
public:
	Anim2Curr();
	Anim2Curr(const Anim2Curr& curr);
	Anim2Curr& operator = (const Anim2Curr& curr);
	Anim2Curr(Anim2Symbol* sym);
	~Anim2Curr();

	bool Update(bool loop = true, int fps = 30);

	int GetCurrFrame() const { return m_frame; }

	uint64_t* GetFramePtr(int idx) {
		if (idx >= 0 && idx < m_frames_ptr.size()) {
			return &m_frames_ptr[idx];
		} else {
			return NULL;
		}
	}

	const rg_skeleton_pose* GetSkPose() const { return m_sk_pose; }

private:
	void ResetTime();

	void UpdateSkeletonPose();

private:
	Anim2Symbol* m_sym;

	int m_frame;

	std::vector<uint64_t> m_frames_ptr;

	float m_start_time, m_curr_time;

	rg_skeleton_pose* m_sk_pose;

}; // Anim2Curr

}

#endif // _SPRITE2_ANIM2_CURR_H_