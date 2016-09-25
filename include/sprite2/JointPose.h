#ifndef _SPRITE2_JOINT_POSE_H_
#define _SPRITE2_JOINT_POSE_H_

#include <CU_Uncopyable.h>
#include <SM_Vector.h>

namespace s2
{

class JointPose : private cu::Uncopyable
{
public:
	sm::vec2 trans;
	float rot;
	float scale;

	JointPose();

}; // JointPose

}

#endif // _SPRITE2_JOINT_POSE_H_