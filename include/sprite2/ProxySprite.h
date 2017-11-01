#ifndef _SPRITE2_PROXY_SPRITE_H_
#define _SPRITE2_PROXY_SPRITE_H_

#include "sprite2/Sprite.h"

namespace s2
{

class ProxySprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ProxySprite(const SymPtr& sym);
	
	/**
	 *  @interface
	 *    Sprite
	 */

	virtual void OnMessage(const UpdateParams& up, Message msg);

	virtual bool Update(const UpdateParams& up);
	// proxy's fetch must be a proxy
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const;

	virtual void SetSymbol(const SymPtr& sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const;

	S2_SPR_CLONE_FUNC(ProxySprite)

	VI_DUMMY_FUNC

}; // ProxySprite

}

#endif // _SPRITE2_PROXY_SPRITE_H_
