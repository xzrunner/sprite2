#ifndef _SPRITE2_SHAPE_SPRITE_H_
#define _SPRITE2_SHAPE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ShapeSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ShapeSprite();
	ShapeSprite(const ShapeSprite& spr);
	ShapeSprite& operator = (const ShapeSprite& spr);
	ShapeSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~ShapeSprite();

	SPRITE_CLONE_FUNC(ShapeSprite)

	VI_DUMMY_FUNC

}; // ShapeSprite

}

#endif // _SPRITE2_SHAPE_SPRITE_H_