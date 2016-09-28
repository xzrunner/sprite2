#include "JointMath.h"

#include <SM_Calc.h>

namespace s2
{

JointPose JointMath::Local2World(const JointPose& src_world, const JointPose& local)
{
	JointPose dst;
	dst.rot = src_world.rot + local.rot;
	dst.trans = src_world.trans + sm::rotate_vector(local.trans, dst.rot);
	return dst;
}

sm::vec2 JointMath::World2Local(const JointPose& src_world, const sm::vec2& dst_world)
{
	return sm::rotate_vector(dst_world - src_world.trans, -src_world.rot);
}

}