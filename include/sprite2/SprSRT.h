#ifndef _SPRITE2_SPR_SRT_H_
#define _SPRITE2_SPR_SRT_H_

#include <SM_Vector.h>

namespace s2
{

class SprSRT
{
public:
	enum SRT_IDX
	{
		IDX_POS_X = 0,
		IDX_POS_Y,
		IDX_ANGLE,
		IDX_SCALE_X,
		IDX_SCALE_Y,
		IDX_SHEAR_X,
		IDX_SHEAR_Y,
		IDX_OFFSET_X,
		IDX_OFFSET_Y,
		IDX_CENTER_X,
		IDX_CENTER_Y,

		SRT_MAX
	};

	float srt[SRT_MAX];

public:
	SprSRT();
	SprSRT(const SprSRT& srt);
	SprSRT& operator = (const SprSRT& srt);

	bool operator == (const SprSRT& srt) const;

	void UpdateCenter();

}; // SprSRT

}

#include "sprite2/SprSRT.inl"

#endif // _SPRITE2_SPR_SRT_H_