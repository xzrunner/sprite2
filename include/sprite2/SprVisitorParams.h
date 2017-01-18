#ifndef _SPRITE2_SPR_VISITOR_PARAMS_H_
#define _SPRITE2_SPR_VISITOR_PARAMS_H_

#include "SprTreePath.h"

#include <SM_Matrix.h>

namespace s2
{

class SprVisitorParams
{
public:
	sm::mat4 mt;

	SprTreePath path;

}; // SprVisitorParams

}

#endif // _SPRITE2_SPR_VISITOR_PARAMS_H_