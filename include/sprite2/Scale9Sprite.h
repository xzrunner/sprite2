#ifndef _SPRITE2_SCALE9_SPRITE_H_
#define _SPRITE2_SCALE9_SPRITE_H_

#include "S2_Sprite.h"
#include "Scale9.h"

namespace s2
{

class RenderParams;

class Scale9Sprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Scale9Sprite();
	Scale9Sprite(const Scale9Sprite& spr);
	Scale9Sprite& operator = (const Scale9Sprite& spr);
	Scale9Sprite(Symbol* sym, uint32_t id = -1);
	virtual ~Scale9Sprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Scale9Sprite* Clone() const;

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	const Scale9& GetScale9() const { return m_s9; }

	void Resize(float width, float height);

protected:
	Scale9 m_s9;

	VI_DUMMY_FUNC

}; // Scale9Sprite

}

#endif // _SPRITE2_SCALE9_SPRITE_H_