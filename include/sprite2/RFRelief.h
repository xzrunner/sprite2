#ifndef _SPRITE2_RF_RELIEF_H_
#define _SPRITE2_RF_RELIEF_H_

#include "sprite2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFRelief : public RenderFilter
{
public:
	RFRelief() 
		: RenderFilter(FM_RELIEF) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFRelief(*this);
	}

	virtual size_t Size() const override { return sizeof(RFRelief); }

}; // RFRelief

}

#endif // _SPRITE2_RF_RELIEF_H_