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
	void SetPosition(const sm::vec2& pos) { m_local.position = pos; }

	float GetAngle() const { return m_local.angle; }
	void SetAngle(float angle) { m_local.angle = angle; }

	const sm::vec2& GetScale() const { return m_local.scale; }
	void SetScale(const sm::vec2& scale) { m_local.scale = scale; }	

#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY
	void SetMatrix(const S2_MAT& mat) { m_local.mat = mat; } 
	const S2_MAT& GetMatrix() const { return m_local.mat; }
#endif // S2_SPR_CACHE_LOCAL_MAT_COPY

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