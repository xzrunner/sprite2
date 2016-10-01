#ifndef _SPRITE2_JOINT_POSE_H_
#define _SPRITE2_JOINT_POSE_H_

#include <SM_Vector.h>

namespace s2
{

class JointPose
{
public:
	sm::vec2 trans;
	float rot;
	float scale;

	JointPose();
	JointPose(const sm::vec2& trans, float rot);

	JointPose operator - () const;

	void Lerp(const JointPose& begin, const JointPose& end, float process);

}; // JointPose

JointPose local2world(const JointPose& src, const JointPose& local);
JointPose world2local(const JointPose& src, const JointPose& dst);

}

#endif // _SPRITE2_JOINT_POSE_H_