#ifndef _SPRITE2_SPR_GEO_H_
#define _SPRITE2_SPR_GEO_H_

#include "pre_defined.h"
#include "ObjectPool.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

namespace s2
{

class SprGeo
{
public:
	const sm::vec2& GetPosition() const { return m_state.local.position; }
	void SetPosition(const sm::vec2& pos) { m_state.local.position = pos; }

	float GetAngle() const { return m_state.local.angle; }
	void SetAngle(float angle) { m_state.local.angle = angle; }

	const sm::vec2& GetScale() const { return m_state.local.scale; }
	void SetScale(const sm::vec2& scale) { m_state.local.scale = scale; }	

	const sm::vec2& GetShear() const { return m_state.local.shear; }
	void SetShear(const sm::vec2& shear) { m_state.local.shear = shear; }	

	const sm::vec2& GetOffset() const { return m_state.local.offset; }
	void SetOffset(const sm::vec2& offset) { m_state.local.offset = offset; }

// 	void SetWorldMat(const sm::mat4& mat);
// 	sm::mat4 GetWorldMat();

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
 	SprGeo* GetNext() const { return m_state.next; }
 	void SetNext(SprGeo* next) { m_state.next = next; }

private:
	SprGeo() {}
 	SprGeo(const SprGeo& geo) { m_state.next = NULL; }
 	~SprGeo() {}

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;
		sm::vec2 shear;

		sm::vec2 offset;

		SRT();

		void Init();
	};

// 	struct Matrix
// 	{
// 		float m[6];
// 
// 		Matrix();
// 	};

private:
	union 
	{
		struct
		{
			SRT    local;
		//	Matrix world;
		};

		SprGeo* next;

	} m_state;

	friend class ObjectPool<SprGeo>;
	friend class SprDefault;

}; // SprGeo

typedef ObjectPool<SprGeo> SprGeoPool;

}

#endif // _SPRITE2_SPR_GEO_H_
