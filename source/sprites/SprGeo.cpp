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
	memset(m_state.mat, 0, sizeof(m_state.mat));
	m_state.mat[0] = m_state.mat[3] = 1;
#endif // S2_SPR_CACHE_LOCAL_MAT_COPY
}

void SprGeo::Term()
{
}

}