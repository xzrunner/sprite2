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
	sm::vec2 scale;

	JointPose();
	JointPose(const sm::vec2& trans, float rot = 0, 
		const sm::vec2& scale = sm::vec2(1, 1));

	JointPose operator - () const;
	JointPose operator + (const JointPose& pose) const;

	void Lerp(const JointPose& begin, const JointPose& end, float process);

}; // JointPose

JointPose local2world(const JointPose& src, const JointPose& local);
JointPose world2local(const JointPose& src, const JointPose& dst);

}

#endif // _SPRITE2_JOINT_POSE_H_