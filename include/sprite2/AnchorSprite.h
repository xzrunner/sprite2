#ifndef _SPRITE2_ANCHOR_SPRITE_H_
#define _SPRITE2_ANCHOR_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Actor;

class AnchorSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnchorSprite() {}
   	AnchorSprite(const SymPtr& sym) : Sprite(sym) {}

	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const;
    virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	ActorConstPtr QueryAnchor(const ActorConstPtr& actor) const;

	SPRITE_CLONE_FUNC(AnchorSprite)

	VI_DUMMY_FUNC

}; // AnchorSprite

}

#endif // _SPRITE2_ANCHOR_SPRITE_H_
