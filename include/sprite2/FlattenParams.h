#ifndef _SPRITE2_FLATTEN_PARAMS_H_
#define _SPRITE2_FLATTEN_PARAMS_H_

#include S2_MAT_HEADER

namespace s2
{

class FlattenParams
{
public:
	FlattenParams();
	FlattenParams(const FlattenParams& params);
	FlattenParams& operator = (const FlattenParams& params);

 	const S2_MAT& GetPrevMat() const { return m_prev_mt; }
 	void SetPrevMat(const S2_MAT& mt) { m_prev_mt = mt; }

	const Sprite* GetSpr() const { return m_spr; }
	void SetSpr(const Sprite* spr) { m_spr = spr; }

	const Actor* GetActor() const { return m_actor; }
	void SetActor(const Actor* actor) { m_actor = actor; }

private:
	S2_MAT m_prev_mt;

	const Sprite* m_spr;
	const Actor*  m_actor;

}; // FlattenParams

}

#include "FlattenParams.inl"

#endif // _SPRITE2_FLATTEN_PARAMS_H_