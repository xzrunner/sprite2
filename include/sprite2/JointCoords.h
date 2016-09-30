#ifndef _SPRITE2_JOINT_COORDS_H_
#define _SPRITE2_JOINT_COORDS_H_

#include <SM_Vector.h>

namespace s2
{

class LocalPose
{
public:
	float trans;
	float rot;
	float scale;

	LocalPose();
	LocalPose(float trans, float rot);

	LocalPose operator - () const;

}; // LocalPose

class WorldPose
{
public:
	sm::vec2 pos;
	float angle;
	float scale;
	
	WorldPose();
	WorldPose(const sm::vec2& pos, float angle);

}; // WorldPose

WorldPose local2world(const WorldPose& src, const LocalPose& local);
LocalPose world2local(const WorldPose& src, const WorldPose& dst);

WorldPose local2world(const WorldPose& src, const sm::vec2& offset);
sm::vec2 world2local(const WorldPose& src, const sm::vec2& dst);

}

#endif // _SPRITE2_JOINT_COORDS_H_