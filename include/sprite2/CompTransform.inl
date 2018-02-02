#pragma once

#include <SM_Calc.h>

namespace s2
{

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