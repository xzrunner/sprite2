#include "ImageSprite.h"
#include "FilterMode.h"
#include "RenderShader.h"
#include "RenderFilter.h"
#include "RenderShader.h"
#include "UpdateParams.h"
#include "S2_Symbol.h"
#include "SprGeo.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>

namespace s2
{

ImageSprite::ImageSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

ImageSprite::ImageSprite(const ImageSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

ImageSprite& ImageSprite::operator = (const ImageSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

ImageSprite::ImageSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

ImageSprite::~ImageSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_IMAGE);
#endif // S2_DISABLE_STATISTICS
}

ImageSprite* ImageSprite::Clone() const
{
	return new ImageSprite(*this);
}

bool ImageSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	//// visible
	//const Actor* actor = up.GetActor();
	//bool visible = actor ? actor->IsVisible() : IsVisible();
	//if (!visible) {
	//	return false;
	//}

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