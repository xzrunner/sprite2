#ifndef _SPRITE2_RF_GAUSSIAN_BLUR_H_
#define _SPRITE2_RF_GAUSSIAN_BLUR_H_

#include "RenderFilter.h"

namespace s2
{

class RFGaussianBlur : public RenderFilter
{
public:
	RFGaussianBlur() 
		: RenderFilter(FM_GAUSSIAN_BLUR)
		, m_iterations(9)
	{}

	virtual bool operator == (const RenderFilter& rf) const {
		return RenderFilter::operator == (rf) &&
			   m_iterations == static_cast<const RFGaussianBlur&>(rf).m_iterations;
	}

	virtual RenderFilter* Clone() const override {
		return new RFGaussianBlur(*this);
	}

	int GetIterations() const { return m_iterations; }
	void SetIterations(int iterations) { m_iterations = iterations; }

private:
	int m_iterations;

}; // RFGaussianBlur

}

#endif // _SPRITE2_RF_GAUSSIAN_BLUR_H_