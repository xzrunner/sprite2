#ifndef _SPRITE2_GROUP_SPRITE_H_
#define _SPRITE2_GROUP_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class GroupSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	GroupSprite::GroupSprite(Symbol* sym);
	
private:

	VI_DUMMY_FUNC

}; // GroupSprite

}

#endif // _SPRITE2_GROUP_SPRITE_H_