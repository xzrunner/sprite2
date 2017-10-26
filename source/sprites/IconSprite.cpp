#include "sprite2/IconSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>

namespace s2
{

IconSprite::IconSprite() 
	: m_process(1)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

IconSprite::IconSprite(const IconSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

IconSprite& IconSprite::operator = (const IconSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

IconSprite::IconSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_process(1)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

IconSprite::~IconSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

}