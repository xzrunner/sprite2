#ifndef _SPRITE2_UPDATE_PARAMS_H_
#define _SPRITE2_UPDATE_PARAMS_H_

#include S2_MAT_HEADER

#include "FilterMode.h"

namespace s2
{

class Actor;
class Sprite;

class UpdateParams
{
public:
	UpdateParams();
	UpdateParams(const Actor* actor);
	UpdateParams(const UpdateParams& params);
	UpdateParams& operator = (const UpdateParams& params);

	void Push(const Sprite* spr);

	void SetPrevMat(const S2_MAT& mat);
	const S2_MAT& GetPrevMat() const;

	FilterMode    GetPrevFilter() const;
	
	void SetActor(const Actor* actor);
	const Actor* GetActor() const;

	void SetForce(bool force) { m_force = force; }
	bool IsForce() const { return m_force; }

private:
	S2_MAT       m_prev_mat;
	FilterMode   m_prev_filter;

	const Actor* m_actor;

	bool         m_force;

}; // UpdateParams

}

#include "UpdateParams.inl"

#endif // _SPRITE2_UPDATE_PARAMS_H_