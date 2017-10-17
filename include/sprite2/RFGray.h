#ifndef _SPRITE2_RF_GRAY_H_
#define _SPRITE2_RF_GRAY_H_

#include "RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFGray : public RenderFilter
{
public:
	RFGray() 
		: RenderFilter(FM_GRAY) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFGray(*this);
	}

	virtual size_t Size() const override { return sizeof(RFGray); }

}; // RFGray

}

#endif // _SPRITE2_RF_GRAY_H_