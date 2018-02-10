#pragma once

#include "sprite2/config.h"

#ifndef S2_DISABLE_FLATTEN

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"
#include "sprite2/RenderReturn.h"

#include S2_MAT_HEADER

#include <memory>

namespace ft { class FTList; }
namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class Actor;
class RenderParams;
class RenderTarget;

class DrawMaskFT
{
public:
	static RenderReturn Draw(cooking::DisplayList* dlist, ft::FTList& ft, 
		int base, int mask, const RenderParams& rp);

private:
	static RenderReturn DrawBaseToRT(cooking::DisplayList* dlist, RenderTarget* rt, 
		ft::FTList& ft, int base,  const Actor* actor, const RenderParams& rp);
	static RenderReturn DrawMaskToRT(cooking::DisplayList* dlist, RenderTarget* rt, 
		ft::FTList& ft, int mask, const Actor* actor, const RenderParams& rp);

	static RenderReturn DrawMaskFromRT(cooking::DisplayList* dlist, RenderTarget* rt_base, 
		RenderTarget* rt_mask, ft::FTList& ft, int mask, const S2_MAT& mt);

}; // DrawMaskFT

}

#endif // S2_DISABLE_FLATTEN