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

	Actor* actor;

public:
	SprVisitorParams() : actor(nullptr) {}

}; // SprVisitorParams

class SprVisitorParams2
{
public:
	S2_MAT mt;

	ActorConstPtr actor;

public:
	SprVisitorParams2() : actor(nullptr) {}

}; // SprVisitorParams2

}

#endif // _SPRITE2_SPR_VISITOR_PARAMS_H_