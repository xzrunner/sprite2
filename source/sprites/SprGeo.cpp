#include "SprGeo.h"

#include <string.h>

namespace s2
{

SprGeo::SprGeo() 
{ 
	Init(); 
}

SprGeo::SprGeo(const SprGeo& geo) 
{ 
	memcpy(this, &geo, sizeof(*this));
}

SprGeo& SprGeo::operator = (const SprGeo& geo)
{
	memcpy(this, &geo, sizeof(*this));
	return *this;
}

SprGeo::~SprGeo() 
{
}

void SprGeo::Init()
{
	m_state.srt.Init();
#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY
	m_state.mat.Init();
#endif // S2_SPR_CACHE_LOCAL_MAT_COPY
}

void SprGeo::Term()
{
}

/************************************************************************/
/* class SprGeo::SRT                                                    */
/************************************************************************/

SprGeo::SRT::SRT()
	: position(0, 0)
	, angle(0)
	, scale(1, 1)
	, shear(0, 0)
	, offset(0, 0)
	, center(0, 0)
{
}

void SprGeo::SRT::Init()
{
	position.Set(0, 0);
	angle = 0;
	scale.Set(1, 1);
	shear.Set(0, 0);
	offset.Set(0, 0);
	center.Set(0, 0);
}

#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY

/************************************************************************/
/* class SprGeo::MAT                                                    */
/************************************************************************/

SprGeo::MAT::MAT()
{
	Init();
}

void SprGeo::MAT::Init()
{
	m[0] = m[3] = 1;
	m[1] = m[2] = m[4] = m[5] = 0;
}

#endif // S2_SPR_CACHE_LOCAL_MAT_COPY

}