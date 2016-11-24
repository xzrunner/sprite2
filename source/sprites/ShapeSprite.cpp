#include "ShapeSprite.h"

#include <stddef.h>

namespace s2
{

ShapeSprite::ShapeSprite() 
{
}

ShapeSprite::ShapeSprite(Symbol* sym, uint32_t id) 
	: Sprite(sym, id) 
{
}

ShapeSprite* ShapeSprite::Clone() const
{
	return new ShapeSprite(*this);
}

}