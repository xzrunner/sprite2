#ifndef _SPRITE2_RF_SHOCK_WAVE_H_
#define _SPRITE2_RF_SHOCK_WAVE_H_

#include "sprite2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFShockWave : public RenderFilter
{
public:
	RFShockWave() 
		: RenderFilter(FM_SHOCK_WAVE) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFShockWave(*this);
	}

	virtual size_t Size() const override { return sizeof(RFShockWave); }

}; // RFShockWave

}

#endif // _SPRITE2_RF_SHOCK_WAVE_H_