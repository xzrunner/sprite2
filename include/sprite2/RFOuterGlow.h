#ifndef _SPRITE2_RF_OUTER_GLOW_H_
#define _SPRITE2_RF_OUTER_GLOW_H_

#include "sprite2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFOuterGlow : public RenderFilter
{
public:
	RFOuterGlow() 
		: RenderFilter(FM_OUTER_GLOW) 
		, m_iterations(9)
	{}

	virtual bool operator == (const RenderFilter& rf) const override {
		return RenderFilter::operator == (rf) &&
			   m_iterations == static_cast<const RFOuterGlow&>(rf).m_iterations;
	}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFOuterGlow(*this);
	}

	virtual size_t Size() const override { return sizeof(RFOuterGlow); }

	int GetIterations() const { return m_iterations; }
	void SetIterations(int iterations) { m_iterations = iterations; }

private:
	int m_iterations;

}; // RFOuterGlow

}

#endif // _SPRITE2_RF_OUTER_GLOW_H_