#include "JointPose.h"

namespace s2
{

JointPose::JointPose()
	: rot(0)
	, scale(1)
{
}

JointPose::JointPose(sm::vec2 trans, float rot)
	: trans(trans)
	, rot(rot)
	, scale(1) 
{
}

}