#ifndef _SPRITE2_SPR_SRT_INL_
#define _SPRITE2_SPR_SRT_INL_

#include "string.h"

#include <SM_Calc.h>

namespace s2
{

inline
SprSRT::SprSRT()
{
	memset(srt, 0, sizeof(srt));
	srt[IDX_SCALE_X] = 1;
	srt[IDX_SCALE_Y] = 1;
}

inline
SprSRT::SprSRT(const SprSRT& srt)
{
	memcpy(this->srt, srt.srt, sizeof(this->srt));
}

inline
SprSRT& SprSRT::operator = (const SprSRT& srt)
{
	memcpy(this->srt, srt.srt, sizeof(this->srt));
	return *this;
}

inline
void SprSRT::UpdateCenter()
{
	sm::vec2 position(srt[IDX_POS_X], srt[IDX_POS_Y]);
	sm::vec2 offset(srt[IDX_OFFSET_X], srt[IDX_OFFSET_Y]);
	float angle = srt[IDX_ANGLE];
	sm::vec2 center = position + sm::rotate_vector(-offset, angle) + offset;
	srt[IDX_CENTER_X] = center.x;
	srt[IDX_CENTER_Y] = center.y;
}

}

#endif // _SPRITE2_SPR_SRT_INL_