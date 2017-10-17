#include "FilterFactory.h"

#include "RFEdgeDetection.h"
#include "RFRelief.h"
#include "RFOutline.h"
#include "RFOuterGlow.h"

#include "RFGray.h"
#include "RFBlur.h"
#include "RFGaussianBlur.h"

#include "RFHeatHaze.h"
#include "RFShockWave.h"
#include "RFSwirl.h"
#include "RFBurningMap.h"

#include "RFColGrading.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>

namespace s2
{

CU_SINGLETON_DEFINITION(FilterFactory)

FilterFactory::FilterFactory() 
{
	memset(m_temps, 0, sizeof(m_temps));

	m_temps[FM_EDGE_DETECTION]	= new RFEdgeDetection;
	m_temps[FM_RELIEF]			= new RFRelief;
	m_temps[FM_OUTLINE]			= new RFOutline;
	m_temps[FM_OUTER_GLOW]		= new RFOuterGlow;

	m_temps[FM_GRAY]			= new RFGray;
	m_temps[FM_BLUR]			= new RFBlur;
	m_temps[FM_GAUSSIAN_BLUR]	= new RFGaussianBlur;

	m_temps[FM_HEAT_HAZE]		= new RFHeatHaze;
	m_temps[FM_SHOCK_WAVE]		= new RFShockWave;
	m_temps[FM_SWIRL]			= new RFSwirl;
	m_temps[FM_BURNING_MAP]		= new RFBurningMap;	

	m_temps[FM_COL_GRADING]		= new RFColGrading;
}

FilterFactory::~FilterFactory() 
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		if (m_temps[i]) {
			delete m_temps[i];
		}
	}
}

RenderFilterPtr FilterFactory::Create(FilterMode mode)
{
	RenderFilter* filter = nullptr;
	switch (mode)
	{
	case FM_NULL:
		break;

	case FM_EDGE_DETECTION:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFEdgeDetection))) RFEdgeDetection();
		break;
	case FM_RELIEF:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFRelief))) RFRelief();
		break;
	case FM_OUTLINE:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFOutline))) RFOutline();
		break;
	case FM_OUTER_GLOW:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFOuterGlow))) RFOuterGlow();
		break;

	case FM_GRAY:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFGray))) RFGray();
		break;
	case FM_BLUR:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFBlur))) RFBlur();
		break;
	case FM_GAUSSIAN_BLUR:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFGaussianBlur))) RFGaussianBlur();
		break;

	case FM_HEAT_HAZE:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFHeatHaze))) RFHeatHaze();
		break;
	case FM_SHOCK_WAVE:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFShockWave))) RFShockWave();
		break;
	case FM_SWIRL:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFSwirl))) RFSwirl();
		break;
	case FM_BURNING_MAP:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFBurningMap))) RFBurningMap();
		break;

	case FM_COL_GRADING:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFColGrading))) RFColGrading();
		break;

	default:
		assert(0);
	}

	return RenderFilterPtr(filter, RenderFilter::Deleter);
}

}