#ifndef _SPRITE2_DRAW_MASK_FT_H_
#define _SPRITE2_DRAW_MASK_FT_H_

#include "pre_defined.h"
#include "s2_typedef.h"
#include "RenderReturn.h"

#include S2_MAT_HEADER

#include <memory>

namespace ft { class FTList; }

namespace s2
{

class Sprite;
class Actor;
class RenderParams;
class RenderColor;
class RenderTarget;

class DrawMaskFT
{
public:
	static RenderReturn Draw(ft::FTList& ft, int base, int mask, const RenderParams& rp);

private:
	static RenderReturn DrawBaseToRT(RenderTarget* rt, ft::FTList& ft, int base, 
		const Actor* actor, const RenderParams& rp);
	static RenderReturn DrawMaskToRT(RenderTarget* rt, ft::FTList& ft, int mask, 
		const Actor* actor, const RenderParams& rp);

	static RenderReturn DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, ft::FTList& ft, int mask, const S2_MAT& mt);

}; // DrawMaskFT

}

#endif // _SPRITE2_DRAW_MASK_FT_H_