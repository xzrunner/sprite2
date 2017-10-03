#ifndef _SPRITE2_ANIM2_CURR_H_
#define _SPRITE2_ANIM2_CURR_H_

#include <vector>
#include <memory>

#include <stdint.h>
#include <stddef.h>

struct rg_skeleton_pose;
struct rg_skeleton_skin;

namespace s2
{

class Anim2Symbol;

class Anim2Curr
{
public:
	Anim2Curr();
	Anim2Curr(const Anim2Curr& curr);
	Anim2Curr& operator = (const Anim2Curr& curr);
	Anim2Curr(const std::shared_ptr<Anim2Symbol>& sym);
	~Anim2Curr();

	bool Update(bool loop = true, int fps = 30);

	int GetCurrFrame() const { return m_frame; }
	void SetStaticCurrFrame(int frame);

	uint64_t* GetFramePtr(int idx) {
		if (idx >= 0 && idx < static_cast<int>(m_frames_ptr.size())) {
			return &m_frames_ptr[idx];
		} else {
			return nullptr;
		}
	}

	const rg_skeleton_pose* GetSkPose() const { return m_sk_pose; }
	const rg_skeleton_skin* GetSkSkin() const { return m_sk_skin; }

private:
	void ResetTime();

	void UpdateRigging();

private:
	std::weak_ptr<Anim2Symbol> m_sym;

	int m_frame;

	std::vector<uint64_t> m_frames_ptr;

	float m_start_time, m_curr_time;

	rg_skeleton_pose* m_sk_pose;
	rg_skeleton_skin* m_sk_skin;

	bool m_active;

}; // Anim2Curr

}

#endif // _SPRITE2_ANIM2_CURR_H_