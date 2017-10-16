#ifndef _SPRITE2_RF_COL_GRADING_H_
#define _SPRITE2_RF_COL_GRADING_H_

#include "RenderFilter.h"

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
		return new RFColGrading(*this);
	}

	const std::string& GetFilepath() const { return m_filepath; }
	void SetFilepath(const std::string& filepath) { m_filepath = filepath; }

private:
	std::string m_filepath;		// color grading LUT texture

}; // RFColGrading

}

#endif // _SPRITE2_RF_COL_GRADING_H_