#include "ShapeSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>

namespace s2
{

ShapeSprite::ShapeSprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS
}

ShapeSprite::ShapeSprite(const ShapeSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS
}

ShapeSprite& ShapeSprite::operator = (const ShapeSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

ShapeSprite::ShapeSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id) 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS
}

ShapeSprite::~ShapeSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS	
}

}