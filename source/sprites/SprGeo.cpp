#include "sprite2/SprGeo.h"

#include <string.h>

namespace s2
{

SprGeo::SprGeo() 
{ 
	Reset();
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

void SprGeo::Reset()
{
	m_srt.Reset();
	m_mat.Identity();
}

}