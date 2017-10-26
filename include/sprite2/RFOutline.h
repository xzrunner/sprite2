#ifndef _SPRITE2_RF_OUTLINE_H_
#define _SPRITE2_RF_OUTLINE_H_

#include "sprite2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace s2
{

class RFOutline : public RenderFilter
{
public:
	RFOutline() 
		: RenderFilter(FM_OUTLINE) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFOutline(*this);
	}

	virtual size_t Size() const override { return sizeof(RFOutline); }

}; // RFOutline

}

#endif // _SPRITE2_RF_OUTLINE_H_