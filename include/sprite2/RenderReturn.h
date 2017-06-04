#ifndef _SPRITE2_RENDER_RETURN_H_
#define _SPRITE2_RENDER_RETURN_H_

#include <stdint.h>

namespace s2
{

typedef uint32_t RenderReturn;

static const RenderReturn RENDER_OK         = 0;
static const RenderReturn RENDER_UNKNOWN    = 0x00000001;
static const RenderReturn RENDER_OUTSIDE    = 0x00000002;
static const RenderReturn RENDER_INVISIBLE  = 0x00000004;
static const RenderReturn RENDER_NO_RT      = 0x00000008;
static const RenderReturn RENDER_NO_DATA    = 0x00000010;
static const RenderReturn RENDER_ON_LOADING = 0x00000020;
static const RenderReturn RENDER_SKIP       = 0x00000040;

}

#endif // _SPRITE2_RENDER_RETURN_H_