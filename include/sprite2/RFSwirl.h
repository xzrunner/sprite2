#ifndef _SPRITE2_RF_SWIRL_H_
#define _SPRITE2_RF_SWIRL_H_

#include "RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFSwirl : public RenderFilter
{
public:
	RFSwirl() 
		: RenderFilter(FM_SWIRL) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFSwirl(*this);
	}

	virtual size_t Size() const override { return sizeof(RFSwirl); }

}; // RFSwirl

}

#endif // _SPRITE2_RF_SWIRL_H_