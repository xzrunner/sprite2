#ifndef _SPRITE2_SKELETON_POSE_H_
#define _SPRITE2_SKELETON_POSE_H_

#include "JointCoords.h"

#include <CU_Uncopyable.h>

#include <vector>

namespace s2
{

class Skeleton;

class SkeletonPose : private cu::Uncopyable
{
public:
	Skeleton* skeleton;
	std::vector<LocalPose> joints_pose;

	SkeletonPose();

}; // SkeletonPose

}

#endif // _SPRITE2_SKELETON_POSE_H_