#ifndef _SPRITE2_SKELETON_POSE_H_
#define _SPRITE2_SKELETON_POSE_H_

#include "JointPose.h"

#include <cu/cu_stl.h>

namespace s2
{

class Skeleton;

class SkeletonPose
{
public:
	SkeletonPose() {}
	SkeletonPose(const Skeleton& skeleton);

	void LoadFromSkeleton(const Skeleton& skeleton);
	void StoreToSkeleton(const Skeleton& skeleton) const;

	void Lerp(const SkeletonPose& begin, const SkeletonPose& end, float process);

	const CU_VEC<JointPose>& GetJointPose() const { return m_joints_pose; }
	void SetJointPose(const CU_VEC<JointPose>& pose) { m_joints_pose = pose; }

	void SetJointPose(int idx, const JointPose& pose);

private:
	CU_VEC<JointPose> m_joints_pose;

}; // SkeletonPose

}

#endif // _SPRITE2_SKELETON_POSE_H_