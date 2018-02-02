#pragma once

#include "sprite2/Component.h"

#include <painting2/OBB.h>

namespace s2
{

class CompBoundingBox : public Component
{
public:
	virtual ~CompBoundingBox();

	virtual CompBoundingBox* Clone() const;

	const pt2::BoundingBox& GetBB() const { return m_bound; }
	pt2::BoundingBox& GetBB() { return m_bound; }

	static CompBoundingBox* Create();

private:
	pt2::OBB m_bound;

}; // CompBoundingBox

}