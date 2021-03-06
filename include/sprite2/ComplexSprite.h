#ifndef _SPRITE2_COMPLEX_SPRITE_H_
#define _SPRITE2_COMPLEX_SPRITE_H_

#include "sprite2/Sprite.h"

namespace s2
{

class ComplexSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ComplexSprite();
	ComplexSprite(const ComplexSprite& spr);
	ComplexSprite& operator = (const ComplexSprite& spr);
	ComplexSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~ComplexSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg) override;
	virtual bool Update(const UpdateParams& up) override;
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const override;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const override;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const override;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const override;

	void SetAction(int action);
	int GetAction() const { return m_action; }
	
private:
	bool VisitChild(SpriteVisitor& visitor, const SprVisitorParams& params, const SprPtr& child, VisitResult& ret) const;

	int GetAction(const Actor* actor) const;

protected:
	int m_action;

	S2_SPR_CLONE_FUNC(ComplexSprite)

	VI_DUMMY_FUNC

}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_