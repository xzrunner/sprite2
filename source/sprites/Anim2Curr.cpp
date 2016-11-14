#include "Anim2Curr.h"
#include "Anim2Symbol.h"
#include "Animation.h"

#include <rigging/rg_skeleton.h>
#include <rigging/rg_animation.h>
#include <rigging/rg_skeleton_pose.h>
#include <rigging/rg_dopesheet.h>

#include <CU_RefCountObj.h>

#include <assert.h>

namespace s2
{

Anim2Curr::Anim2Curr()
	: m_sym(NULL)
	, m_frame(0)
	, m_sk_pose(NULL)
{
	ResetTime();
}

Anim2Curr::Anim2Curr(const Anim2Curr& curr)
	: m_sym(NULL)
	, m_frame(curr.m_frame)
	, m_frames_ptr(curr.m_frames_ptr)
	, m_start_time(curr.m_start_time)
	, m_curr_time(curr.m_curr_time)
	, m_sk_pose(NULL)
{
	cu::RefCountObjAssign(m_sym, const_cast<Anim2Symbol*>(curr.m_sym));
}

Anim2Curr& Anim2Curr::operator = (const Anim2Curr& curr)
{
	m_sym = NULL;
	cu::RefCountObjAssign(m_sym, const_cast<Anim2Symbol*>(curr.m_sym));
	m_frame = curr.m_frame;
	m_frames_ptr = curr.m_frames_ptr;
	m_start_time = curr.m_start_time;
	m_curr_time = curr.m_curr_time;
	m_sk_pose = NULL;
	return *this;
}

Anim2Curr::Anim2Curr(Anim2Symbol* sym)
	: m_sym(NULL)
	, m_frame(0)
{
	ResetTime();
	cu::RefCountObjAssign(m_sym, sym);

	rg_skeleton* sk = sym->GetAnim()->sk;

	m_frames_ptr.resize(sk->joint_count, 0);

	int sz = SIZEOF_RG_SKELETON_POSE + sizeof(rg_pose_pair) * sk->joint_count;
	m_sk_pose = (rg_skeleton_pose*)malloc(sz);
	memset(m_sk_pose, 0, sz);
}

Anim2Curr::~Anim2Curr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
	if (m_sk_pose) {
		free(m_sk_pose);
	}
}

bool Anim2Curr::Update(bool loop, int fps)
{
	bool dirty = false;

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
	int curr_frame = (m_curr_time - m_start_time) * fps + 1;
	int max_frame = m_sym->GetAnim()->max_frame;
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
		const rg_animation* anim = m_sym->GetAnim();
		// todo: GetFramePtr(i) as dims_ptr
		rg_skeleton_pose_update(m_sk_pose, anim->sk, anim->ds, m_frame);
		dirty = true;
	}

	return dirty;
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

}