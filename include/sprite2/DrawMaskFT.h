#pragma once

#include "sprite2/config.h"

#ifndef S2_DISABLE_FLATTEN

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"

#include S2_MAT_HEADER
#include <painting2/RenderReturn.h>

#include <memory>

namespace ft { class FTList; }
namespace cooking { class DisplayList; }
namespace pt2 { class RenderTarget; }

namespace s2
{

class Sprite;
class Actor;
class RenderParams;

class DrawMaskFT
{
public:
	static pt2::RenderReturn Draw(cooking::DisplayList* dlist, ft::FTList& ft, 
		int base, int mask, const RenderParams& rp);

private:
	static pt2::RenderReturn DrawBaseToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, 
		ft::FTList& ft, int base,  const Actor* actor, const RenderParams& rp);
	static pt2::RenderReturn DrawMaskToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, 
		ft::FTList& ft, int mask, const Actor* actor, const RenderParams& rp);

	static pt2::RenderReturn DrawMaskFromRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt_base, 
		pt2::RenderTarget* rt_mask, ft::FTList& ft, int mask, const S2_MAT& mt);

}; // DrawMaskFT

}

#endif // S2_DISABLE_FLATTEN