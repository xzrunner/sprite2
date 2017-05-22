#ifndef _SPRITE2_ACTOR_GEO_H_
#define _SPRITE2_ACTOR_GEO_H_

#include "pre_defined.h"
#include "ObjectPool2.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

namespace s2
{

class ActorGeo
{
public:
	const sm::vec2& GetPosition() const { return m_srt.position; }
	void SetPosition(const sm::vec2& pos);

	float GetAngle() const { return m_srt.angle; }
	void SetAngle(float angle);

	const sm::vec2& GetScale() const { return m_srt.scale; }
	void SetScale(const sm::vec2& scale);

	const S2_MAT& GetMatrix() const { return m_mat; }

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();

private:
	void UpdateMatrix();

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;

		SRT();

		void Init();

	}; // SRT

private:
	SRT    m_srt;
	S2_MAT m_mat;

}; // ActorGeo

typedef ObjectPool2<ActorGeo> ActorGeoPool;

}

#include "ActorGeo.inl"

#endif // _SPRITE2_ACTOR_GEO_H_