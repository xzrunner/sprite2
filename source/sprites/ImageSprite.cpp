#include "ImageSprite.h"
#include "FilterMode.h"
#include "RenderShader.h"
#include "RenderFilter.h"
#include "RenderShader.h"
#include "RenderParams.h"
#include "S2_Symbol.h"
#include "SprGeo.h"

#include <stddef.h>

namespace s2
{

ImageSprite::ImageSprite()
{
}

ImageSprite::ImageSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
	m_geo->SetOffset(m_sym->GetBounding(this).Center());
}

ImageSprite* ImageSprite::Clone() const
{
	return new ImageSprite(*this);
}

bool ImageSprite::Update(const RenderParams& rp)
{
	RenderShader rs = GetShader() * rp.shader;
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