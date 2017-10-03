#ifndef _SPRITE2_RENDER_FILTER_H_
#define _SPRITE2_RENDER_FILTER_H_

#include "FilterMode.h"

namespace s2
{

class RenderFilter
{
public:
	RenderFilter() : m_mode(FM_NULL) {}
	RenderFilter(const RenderFilter& filter)
		: m_mode(filter.m_mode)
	{}
	RenderFilter(FilterMode mode) 
		: m_mode(mode) 
	{}
	virtual ~RenderFilter() {}

	virtual bool operator == (const RenderFilter& rf) const {
		return GetMode() == rf.GetMode();
	}

	virtual RenderFilter* Clone() const { return nullptr; }

	FilterMode GetMode() const { return m_mode; }

private:
	FilterMode m_mode;

}; // RenderFilter

}

#endif // _SPRITE2_RENDER_FILTER_H_