#include "TextureSprite.h"

#include <stddef.h>

namespace s2
{

TextureSprite::TextureSprite() 
{
}

TextureSprite::TextureSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
}

TextureSprite* TextureSprite::Clone() const
{
	return new TextureSprite(*this);
}

}