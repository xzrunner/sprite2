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
   	AnchorSprite(Symbol* sym) : Sprite(sym) {}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnchorSprite* Clone() const { return new AnchorSprite(*this); }

	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual Sprite* FetchChildByName(int name, const Actor* actor) const;
	virtual Sprite* FetchChildByIdx(int idx, const Actor* actor) const;
    virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	void AddAnchor(const Actor* child, const Actor* parent);
	const Actor* QueryAnchor(const Actor* actor) const;

	VI_DUMMY_FUNC

}; // AnchorSprite

}

#endif // _SPRITE2_ANCHOR_SPRITE_H_
