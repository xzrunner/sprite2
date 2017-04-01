#ifndef _SPRITE2_COMPLEX_SPRITE_H_
#define _SPRITE2_COMPLEX_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ComplexSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ComplexSprite();
	ComplexSprite(Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ComplexSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual bool SetFrame(const UpdateParams& up, int frame, bool force = false);
	virtual Sprite* FetchChild(const std::string& name, const Actor* actor) const;
	virtual Sprite* FetchChild(int idx, const Actor* actor) const;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, SprVisitorParams& params) const;

	void SetAction(int action) { m_action = action; }
	int GetAction() const { return m_action; }
	
private:
	bool VisitChild(SpriteVisitor& visitor, const SprVisitorParams& params, Sprite* child, VisitResult& ret) const;

protected:
	int m_action;

	VI_DUMMY_FUNC

}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_