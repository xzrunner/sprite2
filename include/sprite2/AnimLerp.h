#ifndef _SPRITE2_ANIM_LERP_H_
#define _SPRITE2_ANIM_LERP_H_

#include "sprite2/ILerp.h"

#include <cu/cu_stl.h>

#include <memory>

namespace s2
{

class Sprite;

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
	static void Lerp(const CU_VEC<SprPtr>& begin, const CU_VEC<SprPtr>& end,
		CU_VEC<SprPtr>& tween, int time, int tot_time, const CU_VEC<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps);

	static void Lerp(const Sprite& begin, const Sprite& end, SprPtr& tween, int time, int tot_time,
		const CU_VEC<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps);

	static void LerpSpecial(const Sprite& begin, const Sprite& end, SprPtr& tween, int time, int tot_time);

	static void LerpExpression(const Sprite& begin, const Sprite& end, SprPtr& tween, int time, int tot_time,
		const CU_VEC<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps);

	static bool IsMatched(const Sprite& s0, const Sprite& s1);

}; // AnimLerp

}

#endif // _SPRITE2_ANIM_LERP_H_