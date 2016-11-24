#include "ImageSprite.h"
#include "FilterMode.h"
#include "RenderShader.h"
#include "RenderFilter.h"

#include <stddef.h>

namespace s2
{

ImageSprite::ImageSprite()
{
}

ImageSprite::ImageSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
}

ImageSprite* ImageSprite::Clone() const
{
	return new ImageSprite(*this);
}

bool ImageSprite::Update(const RenderParams& params)
{
	FilterMode filter = m_shader.filter->GetMode();
	return filter == FM_HEAT_HAZE 
		|| filter == FM_SHOCK_WAVE 
		|| filter == FM_SWIRL
		|| filter == FM_BURNING_MAP;
}

}