#ifndef _SPRITE2_SYM_TYPE_H_
#define _SPRITE2_SYM_TYPE_H_

namespace s2
{

enum SymType
{
	SYM_INVALID = 0,
	SYM_UNKNOWN,

	SYM_IMAGE = 10,
	SYM_SCALE9,
	SYM_ICON,
	SYM_TEXTURE,
	SYM_TEXTBOX,
	SYM_COMPLEX,
	SYM_ANIMATION,
	SYM_ANIM2,
	SYM_PARTICLE3D,
	SYM_PARTICLE2D,
	SYM_SHAPE,
	SYM_MESH,
	SYM_MASK,
	SYM_TRAIL,
	SYM_SKELETON,
	SYM_MODEL,
	SYM_AUDIO,

	SYM_SNODE = 98,

	SYM_PROXY = 99,
	SYM_ANCHOR = 100,
};

}

#endif // _SPRITE2_SYM_TYPE_H_