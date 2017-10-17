#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

#include "s2_config.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"
#include "pre_defined.h"
#include "s2_typedef.h"
#include "ObjectPool.h"

#include S2_MAT_HEADER
#include <SM_Rect.h>
#include <cu/cu_macro.h>

#include <memory>

namespace s2
{

class Sprite;
class Actor;

class RenderParams
{
public:
	sm::vec2 vertex_offset;		// for blend

	S2_MAT mt;

	RenderColor	color;
	
//	RenderShader shader;
	RenderFilter* render_filter = nullptr;
	BlendMode     render_blend = BM_NULL;
	FastBlendMode render_fast_blend = FBM_NULL;
	float         render_downsample = 1;

	RenderCamera camera;
	
	const Actor* actor;

	int min_edge;

#ifndef S2_DISABLE_STATISTICS
	int level;
	uint32_t parent_id;
#endif // S2_DISABLE_STATISTICS

private:
	sm::rect view_region;

public:
	RenderParams();
	RenderParams(const RenderParams& params);
	RenderParams& operator = (const RenderParams& params);

	void Reset();
	void ResetRender();

	uint32_t GetFlags() const { return m_flags; }
	void SetFlags(uint32_t flags) { m_flags = flags; }

	void SetViewRegion(float xmin, float ymin, float xmax, float ymax);
	void SetViewRegion(const sm::rect& vr);
	void ClearViewRegion();
	const sm::rect& GetViewRegion() const { return view_region; }

private:
	static const uint32_t FLAG_CHANGE_SHADER        = 0x00000001;
	static const uint32_t FLAG_DISABLE_RENDER_DRAW  = 0x00000002;
	static const uint32_t FLAG_DISABLE_BLEND        = 0x00000004;
	static const uint32_t FLAG_DISABLE_FILTER       = 0x00000008;
	static const uint32_t FLAG_DISABLE_DTEX_C2      = 0x00000010;
	static const uint32_t FLAG_DISABLE_CULLING      = 0x00000020;
	static const uint32_t FLAG_DISABLE_INTEGRATE    = 0x00000040;

	static const uint32_t FLAG_DISABLE_PARTICLE3D   = 0x00000080;

	static const uint32_t FLAG_VIEW_REGION_VALID    = 0x00000100;

	static const uint32_t FLAG_ENABLE_DRAWLIST      = 0x00000200;

public:
	CU_FLAG_METHOD(ChangeShader, FLAG_CHANGE_SHADER)
	CU_FLAG_METHOD(DisableRenderDraw, FLAG_DISABLE_RENDER_DRAW)
	CU_FLAG_METHOD(DisableBlend, FLAG_DISABLE_BLEND)
	CU_FLAG_METHOD(DisableFilter, FLAG_DISABLE_FILTER)
	CU_FLAG_METHOD(DisableDTexC2, FLAG_DISABLE_DTEX_C2)
	CU_FLAG_METHOD(DisableCulling, FLAG_DISABLE_CULLING)
	CU_FLAG_METHOD(DisableIntegrate, FLAG_DISABLE_INTEGRATE)

	CU_FLAG_METHOD(DisableParticle3d, FLAG_DISABLE_PARTICLE3D)

	CU_FLAG_METHOD(ViewRegionValid, FLAG_VIEW_REGION_VALID)

	CU_FLAG_METHOD(EnableDrawlist, FLAG_ENABLE_DRAWLIST)

private:
	mutable uint32_t m_flags;

}; // RenderParams

using RenderParamsPool  = ObjectPool<RenderParams>;
using RenderParamsProxy = ObjProxy<RenderParams>;

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_