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
	virtual CompActorTrans* Clone() const;

	virtual size_t GetSize() const { return sizeof(CompActorTrans); }

	const ActorGeoTrans& GetTrans() const { return m_transform; }
	ActorGeoTrans& GetTrans() { return m_transform; }

	static CompActorTrans* Create();

private:
	ActorGeoTrans m_transform;

}; // CompActorTrans

}