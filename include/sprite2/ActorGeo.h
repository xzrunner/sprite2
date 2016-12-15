#ifndef _SPRITE2_ACTOR_GEO_H_
#define _SPRITE2_ACTOR_GEO_H_

#include <SM_Vector.h>

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

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;

		SRT();
	};

private:
	SRT m_local;

}; // ActorGeo

}

#endif // _SPRITE2_ACTOR_GEO_H_