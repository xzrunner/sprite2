#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"
#include "SprTreePath.h"
#include "pre_defined.h"
#include "s2_macro.h"

#include S2_MAT_HEADER
#include <SM_Rect.h>

namespace s2
{

class Sprite;
class SprTreePath;

class RenderParams
{
public:
	sm::vec2 vertex_offset;		// for blend

	sm::rect view_region;

	S2_MAT mt;

	RenderColor	color;
	
	RenderShader shader;

	RenderCamera camera;
	
	SprTreePath path;

public:
	RenderParams();

private:
	static const uint32_t FLAG_CHANGE_SHADER       = 0x00000001;
	static const uint32_t FLAG_DISABLE_RENDER_DRAW = 0x00000002;
	static const uint32_t FLAG_DISABLE_BLEND       = 0x00000004;
	static const uint32_t FLAG_DISABLE_FILTER      = 0x00000008;
	static const uint32_t FLAG_USE_DTEX            = 0x00000010;

public:
	FLAG_METHOD(ChangeShader, FLAG_CHANGE_SHADER)
	FLAG_METHOD(DisableRenderDraw, FLAG_DISABLE_RENDER_DRAW)
	FLAG_METHOD(DisableBlend, FLAG_DISABLE_BLEND)
	FLAG_METHOD(DisableFilter, FLAG_DISABLE_FILTER)
	FLAG_METHOD(UseDTex, FLAG_USE_DTEX)

private:
	mutable uint32_t m_flags;

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_