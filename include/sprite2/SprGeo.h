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
	SprGeo();
	SprGeo(const SprGeo& geo);
	SprGeo& operator = (const SprGeo& geo);
	~SprGeo();

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
	void SetCenter(const sm::vec2& center);	

#ifdef S2_SPR_CACHE_LOCAL_MAT
	void SetMatrix(const S2_MAT& mat);
	const S2_MAT& GetMatrix() const;
#endif // S2_SPR_CACHE_LOCAL_MAT

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
 	SprGeo* GetNext() const { return m_state.next; }
 	void SetNext(SprGeo* next) { m_state.next = next; }

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;
		sm::vec2 shear;

		sm::vec2 offset;

		sm::vec2 center;

		SRT();

		void Init();
	};

private:
	union 
	{
		struct {
			SRT    srt;
		};
#ifdef S2_SPR_CACHE_LOCAL_MAT
		struct {
			S2_MAT mat;
		};
#endif // S2_SPR_CACHE_LOCAL_MAT
		SprGeo* next;

	} m_state;

}; // SprGeo

typedef ObjectPool<SprGeo> SprGeoPool;

}

#include "SprGeo.inl"

#endif // _SPRITE2_SPR_GEO_H_
