#include "IconSprite.h"

#include <stddef.h>

namespace s2
{

IconSprite::IconSprite() 
	: m_process(1)
{
}

IconSprite::IconSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_process(1)
{
}

IconSprite* IconSprite::Clone() const
{
	return new IconSprite(*this);
}

}