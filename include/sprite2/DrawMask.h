#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include "pre_defined.h"
#include "s2_typedef.h"
#include "RenderReturn.h"

#include S2_MAT_HEADER

#include <memory>

namespace s2
{

class Sprite;
class Actor;
class RenderParams;
class RenderColor;
class RenderTarget;

class DrawMask
{
public:
	static RenderReturn Draw(const SprConstPtr& base, const SprConstPtr& mask, const RenderParams& rp);
//	static RenderReturn DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp);

private:
	static RenderReturn DrawBaseToRT(RenderTarget* rt, const SprConstPtr& base,
		const ActorConstPtr& actor, const RenderParams& rp);
	static RenderReturn DrawMaskToRT(RenderTarget* rt, const SprConstPtr& mask,
		const ActorConstPtr& actor, const RenderParams& rp);

	static RenderReturn DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, const SprConstPtr& mask, const S2_MAT& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_