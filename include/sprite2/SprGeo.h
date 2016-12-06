#ifndef _SPRITE2_SPR_GEO_H_
#define _SPRITE2_SPR_GEO_H_

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace s2
{

class SprGeo
{
public:
	const sm::vec2& GetPosition() const { return m_local.position; }
	void SetPosition(const sm::vec2& pos) { m_local.position = pos; }

	float GetAngle() const { return m_local.angle; }
	void SetAngle(float angle) { m_local.angle = angle; }

	const sm::vec2& GetScale() const { return m_local.scale; }
	void SetScale(const sm::vec2& scale) { m_local.scale = scale; }	

	const sm::vec2& GetShear() const { return m_local.shear; }
	void SetShear(const sm::vec2& shear) { m_local.shear = shear; }	

	const sm::vec2& GetOffset() const { return m_local.offset; }
	void SetOffset(const sm::vec2& offset) { m_local.offset = offset; }

	void SetWorldMat(const sm::mat4& mat);
	sm::mat4 GetWorldMat();

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;
		sm::vec2 shear;

		sm::vec2 offset;

		SRT();
	};

	struct Matrix
	{
		float m[6];

		Matrix();
	};

private:
	SRT    m_local;
	Matrix m_world;

}; // SprGeo

}

#endif // _SPRITE2_SPR_GEO_H_
