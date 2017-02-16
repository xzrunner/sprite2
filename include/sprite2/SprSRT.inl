#ifndef _SPRITE2_SPR_SRT_INL_
#define _SPRITE2_SPR_SRT_INL_

namespace s2
{

inline
SprSRT::SprSRT()
	: position(0, 0)
	, angle(0)
	, scale(1, 1)
	, shear(0, 0)
	, offset(0, 0)
	, center(0, 0)
{
}

inline
void SprSRT::Init()
{
	position.Set(0, 0);
	angle = 0;
	scale.Set(1, 1);
	shear.Set(0, 0);
	offset.Set(0, 0);
	center.Set(0, 0);
}

}

#endif // _SPRITE2_SPR_SRT_INL_