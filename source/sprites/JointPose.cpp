#include "JointPose.h"

#include <SM_Calc.h>

namespace s2
{

JointPose::JointPose()
	: rot(0)
	, scale(1, 1)
{
}

JointPose::JointPose(const sm::vec2& trans, float rot, const sm::vec2& scale)
	: trans(trans)
	, rot(rot)
	, scale(scale)
{
}

JointPose JointPose::operator - () const
{
	JointPose inv;
	inv.trans	= -trans;
	inv.rot		= -rot;
	inv.scale	= sm::vec2(1, 1) / scale;
	return inv;
}

JointPose JointPose::operator + (const JointPose& pose) const
{
	JointPose ret;
	ret.trans	= trans + pose.trans;
	ret.rot		= rot + pose.rot;
	ret.scale	= pose.scale;		// todo
	return ret;
}

void JointPose::Lerp(const JointPose& begin, const JointPose& end, float process)
{
	trans	= begin.trans + (end.trans - begin.trans) * process;
	rot		= begin.rot + (end.rot - begin.rot) * process;
	scale	= begin.scale + (end.scale - begin.scale) * process;
}

JointPose local2world(const JointPose& src, const JointPose& local)
{
	JointPose dst;
	dst.scale	= src.scale * local.scale;
	dst.rot		= src.rot + local.rot;
	dst.trans	= src.trans + sm::rotate_vector(local.trans * src.scale, src.rot);
	return dst;
}

JointPose world2local(const JointPose& src, const JointPose& dst)
{
	JointPose local;
	local.scale	= dst.scale / src.scale;
	local.rot	= dst.rot - src.rot;
	local.trans = sm::rotate_vector((dst.trans - src.trans), -src.rot) / src.scale;
	return local;
}

}