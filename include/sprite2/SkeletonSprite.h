#ifndef _SPRITE2_SKELETON_SPRITE_H_
#define _SPRITE2_SKELETON_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/SkeletonPose.h"

namespace s2
{

class SkeletonSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	SkeletonSprite();
	SkeletonSprite(const SkeletonSprite& spr);
	SkeletonSprite& operator = (const SkeletonSprite& spr);
	SkeletonSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~SkeletonSprite();

	const SkeletonPose& GetPose() const { return m_pose; }
	SkeletonPose& GetPose() { return m_pose; }
	void SetPose(const SkeletonPose& pose) { m_pose = pose; }

protected:
	SkeletonPose m_pose;

	S2_SPR_CLONE_FUNC(SkeletonSprite)

	VI_DUMMY_FUNC

}; // SkeletonSprite

}

#endif // _SPRITE2_SKELETON_SPRITE_H_