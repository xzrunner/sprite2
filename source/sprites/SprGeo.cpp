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
	memset(m_state.srt, 0, sizeof(m_state.srt));
	m_state.srt[SprSRT::IDX_SCALE_X] = 1;
	m_state.srt[SprSRT::IDX_SCALE_Y] = 1;

#ifdef S2_SPR_CACHE_LOCAL_MAT_COPY
	m_state.mat[0] = m_state.mat[3] = 1;
	m_state.mat[1] = m_state.mat[2] = m_state.mat[4] = m_state.mat[5] = 0;
#endif // S2_SPR_CACHE_LOCAL_MAT_COPY
}

void SprGeo::Term()
{
}

}