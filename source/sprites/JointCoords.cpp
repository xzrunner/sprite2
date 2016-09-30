#include "JointCoords.h"

#include <SM_Calc.h>

namespace s2
{

/************************************************************************/
/* class JointLocalPose                                                 */
/************************************************************************/

LocalPose::LocalPose()
	: trans(0)
	, rot(0)
	, scale(1)
{
}

LocalPose::LocalPose(float trans, float rot)
	: trans(trans)
	, rot(rot)
	, scale(1) 
{
}

LocalPose LocalPose::operator - () const
{
	LocalPose inv;
	inv.trans = -trans;
	inv.rot = -rot;
	inv.scale = 1.0f / scale;
	return inv;
}

/************************************************************************/
/* class JointWorldPose                                                 */
/************************************************************************/

WorldPose::WorldPose()
	: angle(0)
	, scale(1)
{
}

WorldPose::WorldPose(const sm::vec2& pos, float angle)
	: pos(pos)
	, angle(angle)
	, scale(1)
{
}

/************************************************************************/
/* trans                                                                */
/************************************************************************/

WorldPose local2world(const WorldPose& src, const LocalPose& local)
{
	WorldPose dst;
	dst.angle = src.angle + local.rot;
	dst.pos = src.pos + sm::rotate_vector(sm::vec2(local.trans, 0), dst.angle);
	return dst;
}

LocalPose world2local(const WorldPose& src, const WorldPose& dst)
{
	LocalPose local;
	local.rot = dst.angle - src.angle;
	local.trans = sm::dis_pos_to_pos(src.pos, dst.pos);
	return local;
}

WorldPose local2world(const WorldPose& src, const sm::vec2& offset)
{
	WorldPose dst;
	dst.angle = src.angle;
	dst.pos = src.pos + sm::rotate_vector(offset, dst.angle);
	return dst;
}

sm::vec2 world2local(const WorldPose& src, const sm::vec2& dst)
{
	sm::vec2 offset;
	offset = sm::rotate_vector(dst - src.pos, -src.angle);
	return offset;
}

}