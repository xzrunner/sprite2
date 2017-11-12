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

	virtual void OnMessage(const UpdateParams& up, Message msg) override;

	virtual bool Update(const UpdateParams& up) override;
	// proxy's fetch must be a proxy
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const override;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const override;

	virtual void SetSymbol(const SymPtr& sym) override;

	virtual void SetCenter(const sm::vec2& pos) override;
	virtual void SetPosition(const sm::vec2& pos) override;
	virtual void SetAngle(float angle) override;
	virtual void SetScale(const sm::vec2& scale) override;
	virtual void SetShear(const sm::vec2& shear) override;
	virtual void SetOffset(const sm::vec2& offset) override;

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const override;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const override;

	S2_SPR_CLONE_FUNC(ProxySprite)

	VI_DUMMY_FUNC

}; // ProxySprite

}

#endif // _SPRITE2_PROXY_SPRITE_H_
