#pragma once

#include "sprite2/SprSRT.h"

#include <SM_Calc.h>

namespace s2
{


inline
void CompTransform::ToSprSrt(SprSRT& srt) const
{
	auto& my_srt = m_transform.GetSRT();

	srt.srt[SprSRT::IDX_POS_X] = my_srt.position.x;
	srt.srt[SprSRT::IDX_POS_Y] = my_srt.position.y;

	srt.srt[SprSRT::IDX_ANGLE] = my_srt.angle;

	srt.srt[SprSRT::IDX_SCALE_X] = my_srt.scale.x;
	srt.srt[SprSRT::IDX_SCALE_Y] = my_srt.scale.y;

	srt.srt[SprSRT::IDX_SHEAR_X] = my_srt.shear.x;
	srt.srt[SprSRT::IDX_SHEAR_Y] = my_srt.shear.y;

	srt.srt[SprSRT::IDX_OFFSET_X] = my_srt.offset.x;
	srt.srt[SprSRT::IDX_OFFSET_Y] = my_srt.offset.y;

	srt.srt[SprSRT::IDX_CENTER_X] = my_srt.center.x;
	srt.srt[SprSRT::IDX_CENTER_Y] = my_srt.center.y;
}

inline
void CompTransform::FromSprSrt(const SprSRT& srt)
{
	pt2::SRT my_srt;

	my_srt.position.x = srt.srt[SprSRT::IDX_POS_X];
	my_srt.position.y = srt.srt[SprSRT::IDX_POS_Y];

	my_srt.angle = srt.srt[SprSRT::IDX_ANGLE];
	
	my_srt.scale.x = srt.srt[SprSRT::IDX_SCALE_X];
	my_srt.scale.y = srt.srt[SprSRT::IDX_SCALE_Y];

	my_srt.shear.x = srt.srt[SprSRT::IDX_SHEAR_X];
	my_srt.shear.y = srt.srt[SprSRT::IDX_SHEAR_Y];

	my_srt.offset.x = srt.srt[SprSRT::IDX_OFFSET_X];
	my_srt.offset.y = srt.srt[SprSRT::IDX_OFFSET_Y];

	my_srt.center.x = srt.srt[SprSRT::IDX_CENTER_X];
	my_srt.center.y = srt.srt[SprSRT::IDX_CENTER_Y];

	m_transform.SetSRT(my_srt);
}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE

inline
void CompTransform::SetMatrix(const S2_MAT& mat)
{
	memcpy(m_state.mat, mat.x, sizeof(m_state.mat));
}

inline
S2_MAT CompTransform::GetMatrix() const
{
	S2_MAT mat;
	memcpy(mat.x, m_state.mat, sizeof(m_state.mat));
	return mat;
}

#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE


}