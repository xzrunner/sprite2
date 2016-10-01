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

void JointPose::Lerp(const JointPose& begin, const JointPose& end, float process)
{
	trans = begin.trans + (end.trans - begin.trans) * process;
	rot = begin.rot + (end.rot - begin.rot) * process;
	scale = 1;		// todo
}

JointPose local2world(const JointPose& src, const JointPose& local)
{
	JointPose dst;
	dst.scale = src.scale;	// todo
	dst.rot = src.rot + local.rot;
	dst.trans = src.trans + sm::rotate_vector(local.trans, src.rot);
	return dst;
}

JointPose world2local(const JointPose& src, const JointPose& dst)
{
	JointPose local;
	local.scale = 1;	// todo
	local.rot = dst.rot - src.rot;
	local.trans = sm::rotate_vector(dst.trans - src.trans, -src.rot);
	return local;
}

}