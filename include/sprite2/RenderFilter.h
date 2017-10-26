#ifndef _SPRITE2_RENDER_FILTER_H_
#define _SPRITE2_RENDER_FILTER_H_

#include "sprite2/FilterMode.h"

#include <memory>

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

	virtual RenderFilter* Clone() const = 0;

	virtual size_t Size() const = 0;

	FilterMode GetMode() const { return m_mode; }	

	static void Deleter(RenderFilter* filter);

private:
	FilterMode m_mode;

}; // RenderFilter

using RenderFilterPtr = std::unique_ptr<RenderFilter, decltype(&RenderFilter::Deleter)>;

}

#endif // _SPRITE2_RENDER_FILTER_H_