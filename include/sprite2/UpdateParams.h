#ifndef _SPRITE2_UPDATE_PARAMS_H_
#define _SPRITE2_UPDATE_PARAMS_H_

#include "sprite2/typedef.h"

#include "sprite2/pre_defined.h"
#include "sprite2/config.h"
#include S2_MAT_HEADER

#include "sprite2/FilterMode.h"

#include <memory>

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

	void Reset();

	void Push(const Sprite* spr);

	void SetPrevMat(const S2_MAT& mat);
	const S2_MAT& GetPrevMat() const;

	FilterMode    GetPrevFilter() const;
	
	void SetActor(const Actor* actor) { m_actor = actor; }
	const Actor* GetActor() const { return m_actor; }

	void SetForce(bool force) { m_force = force; }
	bool IsForce() const { return m_force; }

private:
	S2_MAT        m_prev_mat;
	FilterMode    m_prev_filter;

	const Actor*  m_actor;

	bool          m_force;

}; // UpdateParams

}

#include "sprite2/UpdateParams.inl"

#endif // _SPRITE2_UPDATE_PARAMS_H_