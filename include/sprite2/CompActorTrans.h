#pragma once

#include "sprite2/pre_defined.h"
#include "sprite2/Component.h"
#include "sprite2/ActorGeoTrans.h"

#include <SM_Vector.h>

namespace s2
{

class CompActorTrans : public Component
{
public:
	virtual ~CompActorTrans();

	virtual CompActorTrans* Clone() const;

	const ActorGeoTrans& GetTrans() const { return m_transform; }
	ActorGeoTrans& GetTrans() { return m_transform; }

	static CompActorTrans* Create();

private:
	ActorGeoTrans m_transform;

}; // CompActorTrans

}