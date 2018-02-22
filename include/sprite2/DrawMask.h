#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"
#include "sprite2/RenderReturn.h"

#include S2_MAT_HEADER

#include <memory>

namespace cooking { class DisplayList; }
namespace pt2 { class RenderTarget; }

namespace s2
{

class Sprite;
class Actor;
class RenderParams;

class DrawMask
{
public:
	static RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* base, const Sprite* mask, const RenderParams& rp);
//	static RenderReturn DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp);

private:
	static RenderReturn DrawBaseToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, 
		const Sprite* base, const Actor* actor, const RenderParams& rp);
	static RenderReturn DrawMaskToRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, 
		const Sprite* mask, const Actor* actor, const RenderParams& rp);

	static RenderReturn DrawMaskFromRT(cooking::DisplayList* dlist, pt2::RenderTarget* rt_base, pt2::RenderTarget* rt_mask, const Sprite* mask, const S2_MAT& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_