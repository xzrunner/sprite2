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
	JointPose(sm::vec2 trans, float rot);

}; // JointPose

}

#endif // _SPRITE2_JOINT_POSE_H_