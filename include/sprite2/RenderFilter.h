#ifndef _SPRITE2_RENDER_FILTER_H_
#define _SPRITE2_RENDER_FILTER_H_

#include "FilterMode.h"

#include <CU_Cloneable.h>

namespace s2
{

class RenderFilter : public cu::Cloneable
{
public:
	RenderFilter() : m_mode(FM_NULL) {}
	RenderFilter(const RenderFilter& filter)
		: m_mode(filter.m_mode)
	{}
	RenderFilter(FilterMode mode) 
		: m_mode(mode) 
	{}

	virtual bool operator == (const RenderFilter& rf) const {
		return GetMode() == rf.GetMode();
	}

	virtual RenderFilter* Clone() const { return NULL; }

	FilterMode GetMode() const { return m_mode; }

private:
	FilterMode m_mode;

}; // RenderFilter

}

#endif // _SPRITE2_RENDER_FILTER_H_