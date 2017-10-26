#ifndef _SPRITE2_RF_COL_GRADING_H_
#define _SPRITE2_RF_COL_GRADING_H_

#include "sprite2/RenderFilter.h"

#include <cu/cu_stl.h>
#include <memmgr/Allocator.h>

namespace s2
{

class RFColGrading : public RenderFilter
{
public:
	RFColGrading() 
		: RenderFilter(FM_COL_GRADING) 
	{}

	virtual bool operator == (const RenderFilter& rf) const {
		return RenderFilter::operator == (rf) &&
			   m_filepath == static_cast<const RFColGrading&>(rf).GetFilepath();
	}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFColGrading(*this);
	}

	virtual size_t Size() const override { return sizeof(RFColGrading); }

	const CU_STR& GetFilepath() const { return m_filepath; }
	void SetFilepath(const CU_STR& filepath) { m_filepath = filepath; }

private:
	CU_STR m_filepath;		// color grading LUT texture

}; // RFColGrading

}

#endif // _SPRITE2_RF_COL_GRADING_H_