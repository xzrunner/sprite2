#include "JointPose.h"

#include <SM_Calc.h>

namespace s2
{

JointPose::JointPose()
	: rot(0)
	, scale(1)
{
}

JointPose::JointPose(const sm::vec2& trans, float rot)
	: trans(trans)
	, rot(rot)
	, scale(1)
{
}

JointPose JointPose::operator - () const
{
	JointPose inv;
	inv.trans = -trans;
	inv.rot = -rot;
	inv.scale = scale;
	return inv;
}

JointPose _local2world(const JointPose& src, const JointPose& local)
{
	JointPose dst;
	dst.scale = src.scale;	// todo
	dst.rot = src.rot + local.rot;
	dst.trans = src.trans + sm::rotate_vector(local.trans, src.rot);
	return dst;
}

JointPose _world2local(const JointPose& src, const JointPose& dst)
{
	JointPose local;
	local.scale = 1;	// todo
	local.rot = dst.rot - src.rot;
	local.trans = sm::rotate_vector(dst.trans - src.trans, -src.rot);
	return local;
}

bool equal(const JointPose& p0, const JointPose& p1)
{
	if (fabs(p0.rot - p1.rot) > 0.1f) {
		return false;
	}
	if (fabs(p0.trans.x - p1.trans.x) > 0.1f || fabs(p0.trans.y - p1.trans.y) > 0.1f) {
		return false;
	}
	return true;
}

JointPose local2world(const JointPose& src, const JointPose& local)
{
	JointPose dst = _local2world(src, local);
	
	JointPose l = _world2local(src, dst);
	if (!equal(l, local)) {
		int zz = 0;
	}

	return dst;
}

JointPose world2local(const JointPose& src, const JointPose& dst)
{
	JointPose local = _world2local(src, dst);

	JointPose d = _local2world(src, local);
	if (!equal(d, dst)) {
		int zz = 0;
	}

	return local;
}

}