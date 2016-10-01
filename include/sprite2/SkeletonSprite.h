#ifndef _SPRITE2_SKELETON_SPRITE_H_
#define _SPRITE2_SKELETON_SPRITE_H_

#include "S2_Sprite.h"
#include "SkeletonPose.h"

namespace s2
{

class SkeletonSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	SkeletonSprite();
	SkeletonSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual SkeletonSprite* Clone() const;

	const SkeletonPose& GetPose() const { return m_pose; }
	SkeletonPose& GetPose() { return m_pose; }
	void SetPose(const SkeletonPose& pose) { m_pose = pose; }

private:
	SkeletonPose m_pose;

	VI_DUMMY_FUNC

}; // SkeletonSprite

}

#endif // _SPRITE2_SKELETON_SPRITE_H_