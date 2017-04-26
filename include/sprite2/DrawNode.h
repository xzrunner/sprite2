#ifndef _SPRITE2_DRAW_NODE_H_
#define _SPRITE2_DRAW_NODE_H_

#include "pre_defined.h"
#include "RenderParams.h"

#include <SM_Vector.h>

namespace s2
{

class Sprite;
class Symbol;
class RenderTarget;

class DrawNode
{
public:
	static void InitCB(void (*after_spr)(const Sprite*, const RenderParams&));
	static void InitDTexCB(void (*prepare_render_params)(const RenderParams& rp, const Sprite* spr, RenderParams& child),
		                   void (*c2_insert_spr)(const Sprite*, int tex_id, int tex_w, int tex_h),
						   const float* c2_query_spr(const Sprite* spr, int* tex_id));

	static bool Prepare(const RenderParams& rp, const Sprite* spr, RenderParams& child);
	static S2_MAT PrepareMat(const RenderParams& rp, const Sprite* spr);

	static void Draw(const Sprite* spr, const RenderParams& rp = RenderParams(), bool culling = true);

	static void Draw(const Symbol* sym, 
		             const RenderParams& rp = RenderParams(),
					 const sm::vec2& pos = sm::vec2(0, 0),
					 float angle = 0,
					 const sm::vec2& scale = sm::vec2(1, 1),
					 const sm::vec2& shear = sm::vec2(0, 0));	

	static void Draw(const Symbol* sym, 
		             const RenderParams& rp,
					 const S2_MAT& mt);

	static void DrawAABB(const Sprite* spr, const RenderParams& rp, const Color& col);

	static bool IsOutsideView(const Sprite* spr, const RenderParams& rp);

private:
	static void DTexDrawSprToRT(const Sprite* spr, const RenderParams& rp, RenderTarget* rt);
	static void DTexDrawSprFromRT(const Sprite* spr, const RenderParams& rp, const float* texcoords, int tex_id);

	static void DrawSprImpl(const Sprite* spr, const RenderParams& rp);

	static void DrawSprImplFinal(const Sprite* spr, const RenderParams& rp);

}; // DrawNode

}

#endif // _SPRITE2_DRAW_NODE_H_