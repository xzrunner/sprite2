#ifndef _SPRITE2_RF_BLUR_H_
#define _SPRITE2_RF_BLUR_H_

#include "RenderFilter.h"

namespace s2
{

class RFBlur : public RenderFilter
{
public:
	RFBlur() 
		: RenderFilter(FM_BLUR) 
	{}

	virtual RenderFilter* Clone() const { return new RFBlur(); }

}; // RFBlur

}

#endif // _SPRITE2_RF_BLUR_H_