#ifndef _SPRITE2_UTILITY_H_
#define _SPRITE2_UTILITY_H_

namespace s2
{

class Utility
{
public:
	static void PrepareMat(const S2_MAT& parent_mt, const Sprite* spr, const Actor* actor, S2_MAT& dst)
	{
		if (spr->IsMatDisable()) {
			dst = parent_mt;
		} else {
			if (actor && actor->IsGeoDirty()) {
				static S2_MAT tmp;
				sm::Matrix2D::Mul(spr->GetLocalMat(), parent_mt, tmp);
				sm::Matrix2D::Mul(actor->GetLocalMat(), tmp, dst);
			} else {
				sm::Matrix2D::Mul(spr->GetLocalMat(), parent_mt, dst);
			}
		}
	}

}; // Utility

}

#endif // _SPRITE2_UTILITY_H_