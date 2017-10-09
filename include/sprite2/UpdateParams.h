#ifndef _SPRITE2_UPDATE_PARAMS_H_
#define _SPRITE2_UPDATE_PARAMS_H_

#include "s2_typedef.h"

#include "sprite2/pre_defined.h"
#include S2_MAT_HEADER

#include "FilterMode.h"
#include "ObjectPool3.h"

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

	void Push(const Sprite* spr);

	void SetPrevMat(const S2_MAT& mat);
	const S2_MAT& GetPrevMat() const;

	FilterMode    GetPrevFilter() const;
	
	void SetActor(const Actor* actor) { m_actor = actor; }
	const Actor* GetActor() const { return m_actor; }

	void SetForce(bool force) { m_force = force; }
	bool IsForce() const { return m_force; }

	/**
	 *  @interface
	 *    ObjectPool3
	 */
	void Init();
	void Term();
	UpdateParams* GetNext() const { return m_next; }
	void SetNext(UpdateParams* next) { m_next = next; }

private:
	S2_MAT        m_prev_mat;
	FilterMode    m_prev_filter;

	const Actor*  m_actor;

	bool          m_force;

	UpdateParams* m_next;

}; // UpdateParams

typedef ObjectPool3<UpdateParams> UpdateParamsPool;

}

#include "UpdateParams.inl"

#endif // _SPRITE2_UPDATE_PARAMS_H_