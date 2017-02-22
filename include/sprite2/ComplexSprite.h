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
	virtual void OnMessage(Message msg);
	virtual bool Update(const RenderParams& params);
	virtual bool SetFrame(int frame);
	virtual Sprite* FetchChild(const std::string& name) const;
	virtual Sprite* FetchChild(int idx) const;

	void SetAction(const std::string& name);
	void SetAction(int idx) { m_action = idx; }
	int GetAction() const { return m_action; }
	
protected:
	virtual bool TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const;

protected:
	int m_action;

	VI_DUMMY_FUNC

}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_