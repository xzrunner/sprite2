#ifndef _SPRITE2_JOINT_MATH_H_
#define _SPRITE2_JOINT_MATH_H_

#include "JointPose.h"

#include <SM_Vector.h>

namespace s2
{

class JointMath
{
public:
	static JointPose Local2World(const JointPose& src_world, const JointPose& local);
	static sm::vec2 World2Local(const JointPose& src_world, const sm::vec2& dst_world);

}; // Math

}

#endif // _SPRITE2_JOINT_MATH_H_