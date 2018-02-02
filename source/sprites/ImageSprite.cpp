#include "sprite2/ImageSprite.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/Symbol.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <painting2/FilterMode.h>

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

ImageSprite::ImageSprite(const SymPtr& sym, uint32_t id)
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

bool ImageSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	pt2::FilterMode filter_mode = up.GetPrevFilter();
	auto& filter = GetShader().GetFilter();
#ifdef S2_FILTER_FULL
	if (filter && filter->GetMode() != pt2::FM_NULL) {
		filter_mode = filter->GetMode();
	}
#else
	filter_mode = filter;
#endif // S2_FILTER_FULL
	return filter_mode == pt2::FM_HEAT_HAZE 
		|| filter_mode == pt2::FM_SHOCK_WAVE 
		|| filter_mode == pt2::FM_SWIRL
		|| filter_mode == pt2::FM_BURNING_MAP;
	return false;
}

bool ImageSprite::AutoUpdate(const Actor* actor)
{
	// todo
	return false;
}

}