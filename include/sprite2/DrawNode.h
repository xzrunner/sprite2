#ifndef _SPRITE2_DRAW_NODE_H_
#define _SPRITE2_DRAW_NODE_H_

#include "sprite2/pre_defined.h"
#include "sprite2/RenderParams.h"

#include <SM_Vector.h>
#include <painting2/RenderReturn.h>

namespace cooking { class DisplayList; }
namespace pt2 { class RenderTarget; }

namespace s2
{

class Sprite;
class Symbol;

class DrawNode
{
public:
	static void InitCB(void (*after_spr)(const Sprite*, const RenderParams&));
	static void InitDTexCB(void (*prepare_render_params)(const RenderParams& rp, const Sprite* spr, RenderParams& child),
		                   void (*dtex_sym_insert)(uint64_t uid, const sm::rect& bounding, int tex_id, int tex_w, int tex_h),
						   const float* dtex_sym_query(uint64_t uid, int& tex_id, int& block_id));
	static void InitUIDCB(uint64_t (*get_sym_uid)(const Symbol&),
		                  uint64_t (*get_spr_uid)(const Sprite*),
						  uint64_t (*get_actor_uid)(const Actor*));

	static bool Prepare(const RenderParams& rp, const Sprite* spr, RenderParams& child);

	static pt2::RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp = RenderParams());

	static pt2::RenderReturn Draw(const Symbol& sym,
		                     const RenderParams& rp = RenderParams(),
							 const sm::vec2& pos = sm::vec2(0, 0),
							 float angle = 0,
							 const sm::vec2& scale = sm::vec2(1, 1),
							 const sm::vec2& shear = sm::vec2(0, 0));	

	static pt2::RenderReturn Draw(const Symbol& sym,
		                     const RenderParams& rp,
							 const S2_MAT& mt);

	static pt2::RenderReturn DrawAABB(cooking::DisplayList* dlist, const Sprite* spr, 
		                         const RenderParams& rp, const pt2::Color& col);
	
	/**
	 *  @return
	 *    bool, if is outside
	 */
	static bool CullingTestOutside(const Sprite* spr, const RenderParams& rp);

	static pt2::RenderReturn DrawSprToRT(const Sprite* spr, const RenderParams& rp, pt2::RenderTarget* rt);
	static pt2::RenderReturn DrawSprFromRT(const Sprite* spr, const RenderParams& rp, const float* texcoords, int tex_id);
	static pt2::RenderReturn DrawSymToRT(const Symbol& sym, pt2::RenderTarget* rt);

	static pt2::RenderReturn DTexCacheSym(const Symbol& sym);
	static const float* DTexQuerySym(const Symbol& sym, int& tex_id, int& block_id);

private:
	static pt2::RenderReturn DTexCacheSpr(const Sprite* spr, const RenderParams& rp);
	static pt2::RenderReturn DTexQuerySpr(const Sprite* spr, const RenderParams& rp);

	static pt2::RenderReturn DrawSprImpl(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp);

	static pt2::RenderReturn DrawSprImplFinal(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp);

	static bool IsSmall(const Sprite* spr, const Actor* actor, int min_edge);

}; // DrawNode

}

#endif // _SPRITE2_DRAW_NODE_H_