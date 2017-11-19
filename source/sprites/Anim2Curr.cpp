#include "sprite2/Anim2Curr.h"
#include "sprite2/Anim2Symbol.h"
#include "sprite2/Animation.h"

#include <rigging.h>

#include <assert.h>
#include <string.h>

namespace s2
{

Anim2Curr::Anim2Curr()
	: m_frame(0)
	, m_sk_pose(nullptr)
	, m_sk_skin(nullptr)
	, m_active(true)
{
	ResetTime();
}

Anim2Curr::Anim2Curr(const Anim2Curr& curr)
	: m_sym(curr.m_sym)
	, m_frame(curr.m_frame)
	, m_frames_ptr(curr.m_frames_ptr)
	, m_start_time(curr.m_start_time)
	, m_curr_time(curr.m_curr_time)
	, m_sk_pose(nullptr)
	, m_sk_skin(nullptr)
	, m_active(curr.m_active)
{
}

Anim2Curr& Anim2Curr::operator = (const Anim2Curr& curr)
{
	m_sym = curr.m_sym;
	m_frame = curr.m_frame;
	m_frames_ptr = curr.m_frames_ptr;
	m_start_time = curr.m_start_time;
	m_curr_time = curr.m_curr_time;
	m_sk_pose = nullptr;
	m_sk_skin = nullptr;
	m_active = curr.m_active;
	return *this;
}

Anim2Curr::Anim2Curr(const std::shared_ptr<Anim2Symbol>& sym)
	: m_sym(sym)
	, m_frame(0)
	, m_active(true)
{
	ResetTime();

	rg_skeleton* sk = sym->GetAnim()->sk;

	m_frames_ptr.resize(sk->joint_count, 0);

	int pose_sz = SIZEOF_RG_SKELETON_POSE + sizeof(rg_pose_pair) * sk->joint_count;
	m_sk_pose = (rg_skeleton_pose*)malloc(pose_sz);
	memset(m_sk_pose, 0, pose_sz);

	int skin_sz = SIZEOF_RG_SKELETON_SKIN + sizeof(uint16_t) * sk->slot_count;
	m_sk_skin = (rg_skeleton_skin*)malloc(skin_sz);
	memset(m_sk_skin, 0, skin_sz);

	UpdateRigging();
}

Anim2Curr::~Anim2Curr()
{
	if (m_sk_pose) {
		free(m_sk_pose);
	}
	if (m_sk_skin) {
		free(m_sk_skin);
	}
}

bool Anim2Curr::Update(bool loop, int fps)
{
 	// for debug
//	fps = 3;
// 	const rg_animation* anim = m_sym->GetAnim();
// 	m_frame = 12;
// 	UpdateRigging();
// 	return true;

	//////////////////////////////////////////////////////////////////////////

	bool dirty = false;

	if (!m_active) {
		return dirty;
	}

	// update time
	float curr_time = Animation::Instance()->GetTime() /*- m_stop_during*/;
	assert(m_curr_time <= curr_time);
	if (curr_time == m_curr_time) {
		m_curr_time = curr_time;
		return dirty;
	} else {
		m_curr_time = curr_time;
	}

	// update frame
	auto sym_sp = m_sym.lock();
	if (!sym_sp) {
		return dirty;
	}
	int curr_frame = static_cast<int>((m_curr_time - m_start_time) * fps + 1);
	int max_frame = sym_sp->GetAnim()->max_frame;
	int loop_max_frame = max_frame/* + interval * fps*/;
	if (loop) {
		if (curr_frame <= max_frame) {
			;
		} else if (curr_frame > max_frame && curr_frame <= loop_max_frame) {
			curr_frame = 1;
		} else {
			curr_frame = 1;
			m_start_time = m_curr_time;
		}
	} else {
		if (curr_frame > max_frame) {
			curr_frame = max_frame;
		}
	}

	// update curr frame
	if (curr_frame != m_frame) {
		m_frame = curr_frame;
		UpdateRigging();
		dirty = true;
	}

	return dirty;
}

void Anim2Curr::SetStaticCurrFrame(int frame)
{
	m_active = frame == -1;
	m_frame = frame;
	UpdateRigging();
}

//bool Anim2Curr::Update(bool loop, int fps)
//{
//	++m_frame;
//	int max_frame = m_sym->GetAnim()->max_frame;
//	if (m_frame > max_frame) {
//		m_frame = 0;
//	}
//
//	UpdateSkeletonPose();
//
//	return true;
//}

void Anim2Curr::ResetTime()
{
	m_start_time = m_curr_time = Animation::Instance()->GetTime();
}

void Anim2Curr::UpdateRigging()
{
	auto sym_sp = m_sym.lock();
	if (!sym_sp) {
		return;
	}

	const rg_animation* anim = sym_sp->GetAnim();

	// todo: GetFramePtr(i) as dims_ptr
	rg_skeleton_pose_update(m_sk_pose, anim->sk, anim->timeline.joints, m_frame, anim->curves);

	rg_skeleton_skin_update(m_sk_skin, anim->sk, m_sk_pose, anim->timeline.skins, anim->timeline.deforms, m_frame, anim->curves);
}

}