#ifndef _SPRITE2_SPR_VISITOR_PARAMS_H_
#define _SPRITE2_SPR_VISITOR_PARAMS_H_

#include "SprTreePath.h"

#include S2_MAT_HEADER

namespace s2
{

class SprVisitorParams
{
public:
	S2_MAT mt;

	const Actor* prev;

public:
	SprVisitorParams() : prev(NULL) {}

}; // SprVisitorParams

}

#endif // _SPRITE2_SPR_VISITOR_PARAMS_H_