#ifndef _SPRITE2_UTILITY_H_
#define _SPRITE2_UTILITY_H_

#include "sprite2/RenderColor.h"

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

	static void PrepareColor(const RenderColor& parent_rc, const Sprite* spr, const Actor* actor, RenderColor& dst)
	{
		if (spr->IsColorDisable()) {
			dst = parent_rc;
		} else {
			if (actor && actor->IsColorDirty()) {
				static RenderColor tmp;
				RenderColor::Mul(spr->GetColor(), parent_rc, tmp);
				RenderColor::Mul(actor->GetColor(), tmp, dst);
			} else {
				RenderColor::Mul(spr->GetColor(), parent_rc, dst);
			}}
	}

}; // Utility

}

#endif // _SPRITE2_UTILITY_H_