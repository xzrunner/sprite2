#pragma once

#include "sprite2/Component.h"
#include "sprite2/ActorAABB.h"

namespace s2
{

class CompActorAABB : public Component
{
public:
	virtual CompActorAABB* Clone() const;

	virtual size_t GetSize() const { return sizeof(CompActorAABB); }

	const ActorAABB& AABB() const { return m_aabb; }
	ActorAABB& AABB() { return m_aabb; }

	static CompActorAABB* Create();

private:
	ActorAABB m_aabb;

}; // CompActorAABB

}