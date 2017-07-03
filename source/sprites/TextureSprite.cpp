#include "TextureSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>

namespace s2
{

TextureSprite::TextureSprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS
}

TextureSprite::TextureSprite(const TextureSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS
}

TextureSprite& TextureSprite::operator = (const TextureSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

TextureSprite::TextureSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS
}

TextureSprite::~TextureSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS
}

TextureSprite* TextureSprite::Clone() const
{
	return new TextureSprite(*this);
}

}