#include "ImageSprite.h"
#include "FilterMode.h"
#include "RenderShader.h"
#include "RenderFilter.h"
#include "RenderShader.h"
#include "UpdateParams.h"
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

bool ImageSprite::Update(const UpdateParams& up)
{
	FilterMode filter_mode = up.GetPrevFilter();
	const RenderFilter* filter = GetShader().GetFilter();
	if (filter && filter->GetMode() != FM_NULL) {
		filter_mode = filter->GetMode();
	}
	return filter_mode == FM_HEAT_HAZE 
		|| filter_mode == FM_SHOCK_WAVE 
		|| filter_mode == FM_SWIRL
		|| filter_mode == FM_BURNING_MAP;
	return false;
}

}