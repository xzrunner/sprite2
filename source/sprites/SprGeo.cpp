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
}

void SprGeo::Term()
{
}

}