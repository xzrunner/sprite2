#ifndef _SPRITE2_RF_SWIRL_H_
#define _SPRITE2_RF_SWIRL_H_

#include "RenderFilter.h"

namespace s2
{

class RFSwirl : public RenderFilter
{
public:
	RFSwirl() 
		: RenderFilter(FM_SWIRL) 
	{}

	virtual RenderFilter* Clone() const { return new RFSwirl(*this); }

}; // RFSwirl

}

#endif // _SPRITE2_RF_SWIRL_H_