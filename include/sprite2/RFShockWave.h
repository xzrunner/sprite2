#ifndef _SPRITE2_RF_SHOCK_WAVE_H_
#define _SPRITE2_RF_SHOCK_WAVE_H_

#include "RenderFilter.h"

namespace s2
{

class RFShockWave : public RenderFilter
{
public:
	RFShockWave() 
		: RenderFilter(FM_SHOCK_WAVE) 
	{}

	virtual RenderFilter* Clone() const override {
		return new RFShockWave(*this);
	}

}; // RFShockWave

}

#endif // _SPRITE2_RF_SHOCK_WAVE_H_