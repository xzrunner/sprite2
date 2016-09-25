#ifndef _SPRITE2_SKELETON_SPRITE_H_
#define _SPRITE2_SKELETON_SPRITE_H_

#include "S2_Sprite.h"
#include "SkeletonPose.h"

namespace s2
{

class SkeletonSprite : public VIRTUAL_INHERITANCE Sprite
{
public:

private:
	SkeletonPose m_pose;

}; // SkeletonSprite

}

#endif // _SPRITE2_SKELETON_SPRITE_H_