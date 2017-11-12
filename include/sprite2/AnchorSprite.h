#ifndef _SPRITE2_ANCHOR_SPRITE_H_
#define _SPRITE2_ANCHOR_SPRITE_H_

#include "sprite2/Sprite.h"

namespace s2
{

class Actor;

class AnchorSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnchorSprite() {}
   	AnchorSprite(const SymPtr& sym) : Sprite(sym) {}

	virtual void OnMessage(const UpdateParams& up, Message msg) override;
	virtual bool Update(const UpdateParams& up) override;
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const override;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const override;
    virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const override;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const override;

	const Actor* QueryAnchor(const Actor* actor) const;

	S2_SPR_CLONE_FUNC(AnchorSprite)

	VI_DUMMY_FUNC

}; // AnchorSprite

}

#endif // _SPRITE2_ANCHOR_SPRITE_H_
