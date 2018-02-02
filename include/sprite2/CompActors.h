#pragma once

#include "sprite2/SprComponent.h"
#include "sprite2/SprActors.h"

namespace s2
{

class CompActors : public SprComponent
{
public:
	virtual ~CompActors();

	virtual CompActors* Clone() const;

	const SprActors& Actors() const { return m_actors; }
	SprActors& Actors() { return m_actors; }

	static CompActors* Create();

private:
	SprActors m_actors;

}; // CompActors

}