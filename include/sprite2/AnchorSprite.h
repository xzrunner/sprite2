#ifndef _SPRITE2_ANCHOR_SPRITE_H_
#define _SPRITE2_ANCHOR_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

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

	virtual void OnMessage(Message msg, const SprTreePath& path);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& path, bool force = false);
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;
	virtual Sprite* FetchChild(int idx, const SprTreePath& path) const;
    virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	void AddAnchor(const Sprite* anchor, const SprTreePath& path);
	const Sprite* QueryAnchor(const SprTreePath& path) const;

	VI_DUMMY_FUNC

}; // AnchorSprite

}

#endif // _SPRITE2_ANCHOR_SPRITE_H_
