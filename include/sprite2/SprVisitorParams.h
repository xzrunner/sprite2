#ifndef _SPRITE2_SPR_VISITOR_PARAMS_H_
#define _SPRITE2_SPR_VISITOR_PARAMS_H_

#include S2_MAT_HEADER

#include <memory>

#include <cstddef>

namespace s2
{

class Actor;

class SprVisitorParams
{
public:
	S2_MAT mt;

	ActorConstPtr actor;

public:
	SprVisitorParams() : actor(nullptr) {}

}; // SprVisitorParams

}

#endif // _SPRITE2_SPR_VISITOR_PARAMS_H_