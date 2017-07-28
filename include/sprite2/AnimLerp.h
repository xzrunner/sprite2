#ifndef _SPRITE2_ANIM_LERP_H_
#define _SPRITE2_ANIM_LERP_H_

#include <vector>

namespace s2
{

class Sprite;
class ILerp;

class AnimLerp
{
public:
	enum SprData
	{
		SPR_POS = 0,
		SPR_SCALE,
		SPR_ROTATE,
	};

public:
	static void Lerp(const std::vector<Sprite*>& begin, const std::vector<Sprite*>& end, 
		std::vector<Sprite*>& tween, int time, int tot_time, const std::vector<std::pair<SprData, ILerp*> >& lerps);

	static void Lerp(const Sprite* begin, const Sprite* end, Sprite* tween, int time, int tot_time,
		const std::vector<std::pair<SprData, ILerp*> >& lerps);	

	static void LerpSpecial(const Sprite* begin, const Sprite* end, Sprite* tween, int time, int tot_time);	

	static void LerpExpression(const Sprite* begin, const Sprite* end, Sprite* tween, int time, int tot_time,
		const std::vector<std::pair<SprData, ILerp*> >& lerps);

	static bool IsMatched(const Sprite* s0, const Sprite* s1);

}; // AnimLerp

}

#endif // _SPRITE2_ANIM_LERP_H_