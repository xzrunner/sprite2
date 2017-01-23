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

}