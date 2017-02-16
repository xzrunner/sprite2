#ifndef _SPRITE2_SPR_SRT_H_
#define _SPRITE2_SPR_SRT_H_

#include <SM_Vector.h>

namespace s2
{

class SprSRT
{
public:
	sm::vec2 position;
	float    angle;
	sm::vec2 scale;
	sm::vec2 shear;

	sm::vec2 offset;

	sm::vec2 center;

public:
	SprSRT();

	void Init();

}; // SprSRT

}

#include "SprSRT.inl"

#endif // _SPRITE2_SPR_SRT_H_