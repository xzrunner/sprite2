#ifndef _SPRITE2_RF_RELIEF_H_
#define _SPRITE2_RF_RELIEF_H_

#include "RenderFilter.h"

namespace s2
{

class RFRelief : public RenderFilter
{
public:
	RFRelief() 
		: RenderFilter(FM_RELIEF) 
	{}

	virtual RenderFilter* Clone() const { return new RFRelief(*this); }

}; // RFRelief

}

#endif // _SPRITE2_RF_RELIEF_H_