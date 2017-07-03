#ifndef _SPRITE2_TEXTURE_SPRITE_H_
#define _SPRITE2_TEXTURE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class TextureSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TextureSprite();
	TextureSprite(const TextureSprite& spr);
	TextureSprite& operator = (const TextureSprite& spr);
	TextureSprite(Symbol* sym, uint32_t id = -1);
	virtual ~TextureSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TextureSprite* Clone() const;

	VI_DUMMY_FUNC

}; // TextureSprite

}

#endif // _SPRITE2_TEXTURE_SPRITE_H_