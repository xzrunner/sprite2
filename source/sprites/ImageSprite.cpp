#include "ImageSprite.h"
#include "FilterMode.h"
#include "RenderShader.h"
#include "RenderFilter.h"
#include "RenderShader.h"
#include "RenderParams.h"

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
	RenderShader rs = GetShader() * params.shader;
	if (rs.GetFilter()) {
		FilterMode mode = rs.GetFilter()->GetMode();
		return mode == FM_HEAT_HAZE 
			|| mode == FM_SHOCK_WAVE 
			|| mode == FM_SWIRL
			|| mode == FM_BURNING_MAP;
	}
	return false;
}

}