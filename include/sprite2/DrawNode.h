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
						   const float* c2_query_spr(const Sprite*, int* tex_id),
						   void (*c2_insert_sym)(const Symbol*, int tex_id, int tex_w, int tex_h),
						   const float* c2_query_sym(const Symbol*, int* tex_id));

	static bool Prepare(const RenderParams& rp, const Sprite* spr, RenderParams& child);

	static void Draw(const Sprite* spr, const RenderParams& rp = RenderParams());

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
	
	/**
	 *  @return
	 *    bool, if is outside
	 */
	static bool CullingTestOutside(const Sprite* spr, const RenderParams& rp);

	static void DrawSprToRT(const Sprite* spr, const RenderParams& rp, RenderTarget* rt);
	static void DrawSprFromRT(const Sprite* spr, const RenderParams& rp, const float* texcoords, int tex_id);
	static void DrawSymToRT(const Symbol* sym, RenderTarget* rt);

	static void DTexCacheSym(const Symbol* sym);
	static const float* DTexQuerySym(const Symbol* sym, int& tex_id);

private:
	static void DTexCacheSpr(const Sprite* spr, const RenderParams& rp);
	static void DTexQuerySpr(const Sprite* spr, const RenderParams& rp);

	static void DrawSprImpl(const Sprite* spr, const RenderParams& rp);

	static void DrawSprImplFinal(const Sprite* spr, const RenderParams& rp);

}; // DrawNode

}

#endif // _SPRITE2_DRAW_NODE_H_