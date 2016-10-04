#ifndef _SPRITE2_SKELETON_POSE_H_
#define _SPRITE2_SKELETON_POSE_H_

#include "JointPose.h"

#include <vector>

namespace s2
{

class Skeleton;

class SkeletonPose
{
public:
	SkeletonPose() {}
	SkeletonPose(const Skeleton* skeleton);

	void LoadFromSkeleton(const Skeleton* skeleton);
	void StoreToSkeleton(const Skeleton* skeleton) const;

	void Lerp(const SkeletonPose& begin, const SkeletonPose& end, float process);

	const std::vector<JointPose>& GetJointPose() const { return m_joints_pose; }
	void SetJointPose(const std::vector<JointPose>& pose) { m_joints_pose = pose; }

	void SetJointPose(int idx, const JointPose& pose);

private:
	std::vector<JointPose> m_joints_pose;

}; // SkeletonPose

}

#endif // _SPRITE2_SKELETON_POSE_H_