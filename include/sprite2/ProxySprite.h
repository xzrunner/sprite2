#ifndef _SPRITE2_PROXY_SPRITE_H_
#define _SPRITE2_PROXY_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ProxySprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ProxySprite::ProxySprite(Symbol* sym);
	
	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ProxySprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);

	virtual bool Update(const UpdateParams& up);
	virtual Sprite* FetchChild(const std::string& name, const Actor* actor) const;
	virtual Sprite* FetchChild(int idx, const Actor* actor) const;

	virtual void SetSymbol(Symbol* sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

private:

	VI_DUMMY_FUNC

}; // ProxySprite

}

#endif // _SPRITE2_PROXY_SPRITE_H_
