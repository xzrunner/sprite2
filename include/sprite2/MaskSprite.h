#ifndef _SPRITE2_MASK_SPRITE_H_
#define _SPRITE2_MASK_SPRITE_H_

#include "sprite2/Sprite.h"

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
	virtual void OnMessage(const UpdateParams& up, Message msg) override;
	virtual bool Update(const UpdateParams& up) override;
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const override;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const override;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const override;

	S2_SPR_CLONE_FUNC(MaskSprite)

	VI_DUMMY_FUNC

}; // MaskSprite

}

#endif // _SPRITE2_MASK_SPRITE_H_