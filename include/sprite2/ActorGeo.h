#ifndef _SPRITE2_ACTOR_GEO_H_
#define _SPRITE2_ACTOR_GEO_H_

#include "pre_defined.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

namespace s2
{

class ActorGeo
{
public:
	const sm::vec2& GetPosition() const { return m_local.position; }
	void SetPosition(const sm::vec2& pos);

	float GetAngle() const { return m_local.angle; }
	void SetAngle(float angle);

	const sm::vec2& GetScale() const { return m_local.scale; }
	void SetScale(const sm::vec2& scale);

	const S2_MAT& GetMatrix() const { return m_local.mat; }

private:
	void UpdateMatrix();

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;

		S2_MAT mat;

		SRT();
	};

private:
	SRT m_local;

}; // ActorGeo

}

#endif // _SPRITE2_ACTOR_GEO_H_