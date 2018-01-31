#pragma once

#include "sprite2/pre_defined.h"
#include "sprite2/SprComponent.h"

#include S2_MAT_HEADER
#include <SM_Vector.h>
#include <painting2/GeoTransform.h>

namespace s2
{

class SprSRT;

class CompTransform : public SprComponent
{
public:
	virtual ~CompTransform();

	virtual CompTransform* Clone() const;

	const pt2::GeoTransform& GetTrans() const { return m_transform; }
	pt2::GeoTransform& GetTrans() { return m_transform; }

	void ToSprSrt(SprSRT& srt) const;
	void FromSprSrt(const SprSRT& srt);

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void SetMatrix(const S2_MAT& mat);
	S2_MAT GetMatrix() const;
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	static CompTransform* Create();

private:
	pt2::GeoTransform m_transform;

}; // CompTransform

}

#include "sprite2/CompTransform.inl"