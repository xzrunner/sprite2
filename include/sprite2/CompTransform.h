#pragma once

#include "sprite2/pre_defined.h"
#include "sprite2/SprComponent.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

namespace s2
{

class SprSRT;

class CompTransform : public SprComponent
{
public:
	CompTransform();
	virtual ~CompTransform();

	virtual CompTransform* Clone() const;

	void Reset();

	const sm::vec2& GetPosition() const;
	void SetPosition(const sm::vec2& pos);

	float GetAngle() const;
	void SetAngle(float angle);

	const sm::vec2& GetScale() const;
	void SetScale(const sm::vec2& scale);

	const sm::vec2& GetShear() const;
	void SetShear(const sm::vec2& shear);

	const sm::vec2& GetOffset() const;
	void SetOffset(const sm::vec2& offset);

	const sm::vec2& GetCenter() const;

	void GetSRT(SprSRT& srt) const;
	void SetSRT(const SprSRT& srt);

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void SetMatrix(const S2_MAT& mat);
	S2_MAT GetMatrix() const;
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	const S2_MAT& GetMatrix() const { return m_mat; }

	static CompTransform* Create();

private:
	void UpdateCenter();
	void UpdateMatrix();

private:
	struct SRT
	{
		sm::vec2 position;
		float angle;
		sm::vec2 scale;
		sm::vec2 shear;
		sm::vec2 offset;
		sm::vec2 center;

		SRT();

		void Reset();

	}; // SRT

	SRT    m_srt;
	S2_MAT m_mat;

}; // CompTransform

}

#include "sprite2/CompTransform.inl"