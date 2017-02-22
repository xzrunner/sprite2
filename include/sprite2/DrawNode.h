#ifndef _SPRITE2_DRAW_NODE_H_
#define _SPRITE2_DRAW_NODE_H_

#include "pre_defined.h"
#include "RenderParams.h"

#include <SM_Vector.h>

namespace s2
{

class Sprite;
class Symbol;

class DrawNode
{
public:
	static void InitCB(void (*after_spr)(const Sprite*, const RenderParams&));

	static bool Prepare(const RenderParams& parent, const Sprite* spr, RenderParams& child);

	static void Draw(const Sprite* spr, const RenderParams& params = RenderParams(), bool scissor = true);

	static void Draw(const Symbol* sym, 
		             const RenderParams& params = RenderParams(),
					 const sm::vec2& pos = sm::vec2(0, 0),
					 float angle = 0,
					 const sm::vec2& scale = sm::vec2(1, 1),
					 const sm::vec2& shear = sm::vec2(0, 0));	

	static void Draw(const Symbol* sym, 
		const RenderParams& params,
		const S2_MAT& mt);

	static bool IsOutsideView(const Sprite* spr, const RenderParams& params);

private:
	static void DrawSpr(const Sprite* spr, const RenderParams& params);

}; // DrawNode

}

#endif // _SPRITE2_DRAW_NODE_H_