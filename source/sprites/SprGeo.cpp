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
	m_srt.Init();
	m_mat.Identity();
}

void SprGeo::Term()
{
}

}