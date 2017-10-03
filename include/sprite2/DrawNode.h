#ifndef _SPRITE2_DRAW_NODE_H_
#define _SPRITE2_DRAW_NODE_H_

#include "pre_defined.h"
#include "RenderParams.h"
#include "RenderReturn.h"

#include <SM_Vector.h>

namespace s2
{

class Sprite;
class Symbol;
class RenderTarget;

class DrawNode
{
public:
	static void InitCB(void (*after_spr)(const SprConstPtr&, const RenderParams&));
	static void InitDTexCB(void (*prepare_render_params)(const RenderParams& rp, const SprConstPtr& spr, RenderParams& child),
		                   void (*dtex_sym_insert)(uint64_t uid, const sm::rect& bounding, int tex_id, int tex_w, int tex_h),
						   const float* dtex_sym_query(uint64_t uid, int& tex_id, int& block_id));
	static void InitUIDCB(uint64_t (*get_sym_uid)(const SymConstPtr&),
		                  uint64_t (*get_spr_uid)(const SprConstPtr&),
						  uint64_t (*get_actor_uid)(const ActorConstPtr&));

	static bool Prepare(const RenderParams& rp, const SprConstPtr& spr, RenderParams& child);

	static RenderReturn Draw(const SprConstPtr& spr, const RenderParams& rp = RenderParams());

	static RenderReturn Draw(const SymConstPtr& sym,
		                     const RenderParams& rp = RenderParams(),
							 const sm::vec2& pos = sm::vec2(0, 0),
							 float angle = 0,
							 const sm::vec2& scale = sm::vec2(1, 1),
							 const sm::vec2& shear = sm::vec2(0, 0));	

	static RenderReturn Draw(const SymConstPtr& sym,
		                     const RenderParams& rp,
							 const S2_MAT& mt);

	static RenderReturn DrawAABB(const SprConstPtr& spr, const RenderParams& rp, const Color& col);
	
	/**
	 *  @return
	 *    bool, if is outside
	 */
	static bool CullingTestOutside(const SprConstPtr& spr, const RenderParams& rp);

	static RenderReturn DrawSprToRT(const SprConstPtr& spr, const RenderParams& rp, RenderTarget* rt);
	static RenderReturn DrawSprFromRT(const SprConstPtr& spr, const RenderParams& rp, const float* texcoords, int tex_id);
	static RenderReturn DrawSymToRT(const SymConstPtr& sym, RenderTarget* rt);

	static RenderReturn DTexCacheSym(const SymConstPtr& sym);
	static const float* DTexQuerySym(const SymConstPtr& sym, int& tex_id, int& block_id);

private:
	static RenderReturn DTexCacheSpr(const SprConstPtr& spr, const RenderParams& rp);
	static RenderReturn DTexQuerySpr(const SprConstPtr& spr, const RenderParams& rp);

	static RenderReturn DrawSprImpl(const SprConstPtr& spr, const RenderParams& rp);

	static RenderReturn DrawSprImplFinal(const SprConstPtr& spr, const RenderParams& rp);

	static bool IsSmall(const SprConstPtr& spr, const ActorConstPtr& actor, int min_edge);

}; // DrawNode

}

#endif // _SPRITE2_DRAW_NODE_H_