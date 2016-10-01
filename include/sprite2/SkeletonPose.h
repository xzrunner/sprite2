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

private:
	std::vector<JointPose> m_joints_pose;

}; // SkeletonPose

}

#endif // _SPRITE2_SKELETON_POSE_H_