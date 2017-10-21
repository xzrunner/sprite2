#ifndef _SPRITE2_MASK_SPRITE_H_
#define _SPRITE2_MASK_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class MaskSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MaskSprite();
	MaskSprite(const MaskSprite& spr);
	MaskSprite& operator = (const MaskSprite& spr);
	MaskSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~MaskSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const;

	SPRITE_CLONE_FUNC(MaskSprite)

	VI_DUMMY_FUNC

}; // MaskSprite

}

#endif // _SPRITE2_MASK_SPRITE_H_