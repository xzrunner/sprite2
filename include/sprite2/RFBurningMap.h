#ifndef _SPRITE2_RF_BURNING_MAP_H_
#define _SPRITE2_RF_BURNING_MAP_H_

#include "sprite2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFBurningMap : public RenderFilter
{
public:
	RFBurningMap() 
		: RenderFilter(FM_BURNING_MAP) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFBurningMap(*this);
	}

	virtual size_t Size() const override { return sizeof(RFBurningMap); }

}; // RFBurningMap

}

#endif // _SPRITE2_RF_BURNING_MAP_H_