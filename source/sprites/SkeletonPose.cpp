#include "sprite2/SkeletonPose.h"
#include "sprite2/Skeleton.h"
#include "sprite2/Joint.h"

#include <assert.h>

namespace s2
{

SkeletonPose::SkeletonPose(const Skeleton& skeleton)
{
	LoadFromSkeleton(skeleton);
}

void SkeletonPose::LoadFromSkeleton(const Skeleton& skeleton)
{
	auto& joints = skeleton.GetAllJoints();
	m_joints_pose.clear();
	m_joints_pose.reserve(joints.size());
	for (auto& joint : joints) {
		m_joints_pose.push_back(joint->GetLocalPose());		
	}
}

void SkeletonPose::StoreToSkeleton(const Skeleton& skeleton) const
{
	auto& joints = skeleton.GetAllJoints();
	assert(joints.size() == m_joints_pose.size());
	for (int i = 0, n = joints.size(); i < n; ++i) {
		joints[i]->SetLocalPose(m_joints_pose[i]);
	}
	for (auto joint : joints) {
		auto parent = joint->GetParent();
		if (!parent) {
			joint->Update();
		}
	}
}

void SkeletonPose::Lerp(const SkeletonPose& begin, const SkeletonPose& end, float process)
{
	assert(begin.m_joints_pose.size() == end.m_joints_pose.size()
		&& begin.m_joints_pose.size() == m_joints_pose.size());
	for (int i = 0, n = m_joints_pose.size(); i < n; ++i) {
		m_joints_pose[i].Lerp(begin.m_joints_pose[i], end.m_joints_pose[i], process);
	}
}

void SkeletonPose::SetJointPose(int idx, const JointPose& pose) 
{
	if (idx >= 0 && idx < static_cast<int>(m_joints_pose.size())) {
		m_joints_pose[idx] = pose;
	}
}

}