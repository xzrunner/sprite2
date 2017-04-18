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

	void Push(const Sprite* spr, const Actor* actor = NULL);

	const S2_MAT& GetMat() const { return m_mt; }

	const Sprite* GetSpr() const { return m_spr; }
	const Actor* GetActor() const { return m_actor; }

private:
	S2_MAT m_mt;

	const Sprite* m_spr;
	const Actor*  m_actor;

}; // FlattenParams

}

#include "FlattenParams.inl"

#endif // _SPRITE2_FLATTEN_PARAMS_H_