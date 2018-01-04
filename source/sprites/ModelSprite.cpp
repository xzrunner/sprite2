#include "sprite2/ModelSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

namespace s2
{

ModelSprite::ModelSprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

ModelSprite::ModelSprite(const ModelSprite& spr)
	: Sprite(spr)
	, m_pos3(spr.m_pos3)
	, m_ori3(spr.m_ori3)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

ModelSprite& ModelSprite::operator = (const ModelSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

ModelSprite::ModelSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

ModelSprite::~ModelSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

}