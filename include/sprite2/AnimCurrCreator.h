#ifndef _SPRITE2_ANIM_CURR_CREATOR_H_
#define _SPRITE2_ANIM_CURR_CREATOR_H_

namespace s2
{

class AnimCurr;
class AnimSymbol;
class Sprite;

class AnimCurrCreator
{
public:
	static AnimCurr* Create(const AnimSymbol* sym, const Sprite* spr);

}; // AnimCurrCreator

}

#endif // _SPRITE2_ANIM_CURR_CREATOR_H_