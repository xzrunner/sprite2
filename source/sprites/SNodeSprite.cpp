#include "sprite2/SNodeSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

namespace s2
{

SNodeSprite::SNodeSprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

SNodeSprite::SNodeSprite(const SNodeSprite& spr)
	: Sprite(spr)
	, m_pos3(spr.m_pos3)
	, m_ori3(spr.m_ori3)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

SNodeSprite& SNodeSprite::operator = (const SNodeSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

SNodeSprite::SNodeSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

SNodeSprite::~SNodeSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_MODEL);
#endif // S2_DISABLE_STATISTICS
}

void SNodeSprite::Rotate3(const sm::Quaternion& delta)
{
	m_ori3 = delta.Rotated(m_ori3);
}

}