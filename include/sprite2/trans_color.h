#ifndef _SPRITE2_TRANS_COLOR_H_
#define _SPRITE2_TRANS_COLOR_H_

#include <stdint.h>

namespace s2
{

// text		RGBA	old
// text		ARGB
// add		ABGR
// multi	BGRA
enum PIXEL_TYPE
{
	RGBA = 0,
	ARGB,
	ABGR,
	BGRA
};

uint32_t trans_color(uint32_t src, PIXEL_TYPE src_type, PIXEL_TYPE dst_type);

}

#include "sprite2/trans_color.inl"

#endif // _SPRITE2_TRANS_COLOR_H_