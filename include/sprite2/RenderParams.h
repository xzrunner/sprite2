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
	static const uint32_t FLAG_CHANGE_SHADER        = 0x00000001;
	static const uint32_t FLAG_DISABLE_RENDER_DRAW  = 0x00000002;
	static const uint32_t FLAG_DISABLE_BLEND        = 0x00000004;
	static const uint32_t FLAG_DISABLE_FILTER       = 0x00000008;
	static const uint32_t FLAG_DISABLE_DTEX_C2      = 0x00000010;

public:
	S2_FLAG_METHOD(ChangeShader, FLAG_CHANGE_SHADER)
	S2_FLAG_METHOD(DisableRenderDraw, FLAG_DISABLE_RENDER_DRAW)
	S2_FLAG_METHOD(DisableBlend, FLAG_DISABLE_BLEND)
	S2_FLAG_METHOD(DisableFilter, FLAG_DISABLE_FILTER)
	S2_FLAG_METHOD(DisableDTexC2, FLAG_DISABLE_DTEX_C2)

private:
	mutable uint32_t m_flags;

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_