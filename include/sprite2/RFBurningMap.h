#ifndef _SPRITE2_RF_BURNING_MAP_H_
#define _SPRITE2_RF_BURNING_MAP_H_

#include "RenderFilter.h"

namespace s2
{

class RFBurningMap : public RenderFilter
{
public:
	RFBurningMap() 
		: RenderFilter(FM_BURNING_MAP) 
	{}

	virtual RenderFilter* Clone() const override {
		return new RFBurningMap(*this);
	}

}; // RFBurningMap

}

#endif // _SPRITE2_RF_BURNING_MAP_H_