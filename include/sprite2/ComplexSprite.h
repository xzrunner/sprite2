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
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& parent_path);
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;
	virtual Sprite* FetchChild(int idx, const SprTreePath& path) const;
	virtual void Mount(const std::string& name, const Sprite* anchor, const SprTreePath& path);
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

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