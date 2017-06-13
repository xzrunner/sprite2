#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

#include "s2_config.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"
#include "pre_defined.h"
#include "s2_macro.h"
#include "ObjectPool3.h"

#include S2_MAT_HEADER
#include <SM_Rect.h>

namespace s2
{

class Sprite;
class Actor;

class RenderParams
{
public:
	sm::vec2 vertex_offset;		// for blend

	sm::rect view_region;

	S2_MAT mt;

	RenderColor	color;
	
	RenderShader shader;

	RenderCamera camera;
	
	const Actor* actor;

	int min_edge;

#ifndef S2_DISABLE_STATISTICS
	int level;
	uint32_t parent_id;
#endif // S2_DISABLE_STATISTICS

public:
	RenderParams();
	RenderParams(const RenderParams& params);
	RenderParams& operator = (const RenderParams& params);

	void Reset();

	uint32_t GetFlags() const { return m_flags; }
	void SetFlags(uint32_t flags) { m_flags = flags; }

	/**
	 *  @interface
	 *    ObjectPool3
	 */
	RenderParams* GetNext() const { return m_next; }
	void SetNext(RenderParams* next) { m_next = next; }

private:
	static const uint32_t FLAG_CHANGE_SHADER        = 0x00000001;
	static const uint32_t FLAG_DISABLE_RENDER_DRAW  = 0x00000002;
	static const uint32_t FLAG_DISABLE_BLEND        = 0x00000004;
	static const uint32_t FLAG_DISABLE_FILTER       = 0x00000008;
	static const uint32_t FLAG_DISABLE_DTEX_C2      = 0x00000010;
	static const uint32_t FLAG_DISABLE_CULLING      = 0x00000020;

	static const uint32_t FLAG_DISABLE_PARTICLE3D   = 0x00000040;

public:
	S2_FLAG_METHOD(ChangeShader, FLAG_CHANGE_SHADER)
	S2_FLAG_METHOD(DisableRenderDraw, FLAG_DISABLE_RENDER_DRAW)
	S2_FLAG_METHOD(DisableBlend, FLAG_DISABLE_BLEND)
	S2_FLAG_METHOD(DisableFilter, FLAG_DISABLE_FILTER)
	S2_FLAG_METHOD(DisableDTexC2, FLAG_DISABLE_DTEX_C2)
	S2_FLAG_METHOD(DisableCulling, FLAG_DISABLE_CULLING)

	S2_FLAG_METHOD(DisableParticle3d, FLAG_DISABLE_PARTICLE3D)

private:
	mutable uint32_t m_flags;

	RenderParams* m_next;

}; // RenderParams

typedef ObjectPool3<RenderParams> RenderParamsPool;

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_