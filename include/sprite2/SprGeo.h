#ifndef _SPRITE2_SPR_GEO_H_
#define _SPRITE2_SPR_GEO_H_

#include "pre_defined.h"
#include "ObjectPool.h"
#include "SprSRT.h"

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

	sm::vec2 GetPosition() const;
	void SetPosition(const sm::vec2& pos);

	float GetAngle() const;
	void SetAngle(float angle);

	sm::vec2 GetScale() const;
	void SetScale(const sm::vec2& scale);

	sm::vec2 GetShear() const;
	void SetShear(const sm::vec2& shear);

	sm::vec2 GetOffset() const;
	void SetOffset(const sm::vec2& offset);

	sm::vec2 GetCenter() const;
	void UpdateCenter();	

	void GetSRT(SprSRT& srt) const;
	void SetSRT(const SprSRT& srt);

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void SetMatrix(const S2_MAT& mat);
	S2_MAT GetMatrix() const;
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY
	void SetMatrix(const S2_MAT& mat);
	S2_MAT GetMatrix() const;
#endif // S2_SPR_CACHE_LOCAL_MAT_COPY

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
 	SprGeo* GetNext() const { return m_state.next; }
 	void SetNext(SprGeo* next) { m_state.next = next; }

private:
	union
	{
		struct {
			float srt[SprSRT::SRT_MAX];

#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY
			float mat[6];
#endif // S2_SPR_CACHE_LOCAL_MAT_COPY
		};

 #ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
 		struct {
 			float mat[6];
 		};
 #endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

		SprGeo* next;
	} m_state;

}; // SprGeo

typedef ObjectPool<SprGeo> SprGeoPool;

}

#include "SprGeo.inl"

#endif // _SPRITE2_SPR_GEO_H_
