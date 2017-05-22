#ifndef _SPRITE2_SPR_GEO_H_
#define _SPRITE2_SPR_GEO_H_

#include "pre_defined.h"
#include "ObjectPool2.h"
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

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();

private:
	void UpdateCenter();
	void UpdateMatrix();

private:
// 	union
// 	{
// 		struct {
// 			float srt[SprSRT::SRT_MAX];
// 
// #ifdef S2_SPR_CACHE_LOCAL_MAT_COPY
// 			float mat[6];
// #endif // S2_SPR_CACHE_LOCAL_MAT_COPY
// 		};
// 
//  #ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
//  		struct {
//  			float mat[6];
//  		};
//  #endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
// 
// 		SprGeo* next;
// 	} m_state;

	//////////////////////////////////////////////////////////////////////////

	struct SRT
	{
		sm::vec2 position;
		float angle;
		sm::vec2 scale;
		sm::vec2 shear;
		sm::vec2 offset;
		sm::vec2 center;

		SRT();

	}; // SRT

	SRT    m_srt;
	S2_MAT m_mat;

}; // SprGeo

typedef ObjectPool2<SprGeo> SprGeoPool;

}

#include "SprGeo.inl"

#endif // _SPRITE2_SPR_GEO_H_
