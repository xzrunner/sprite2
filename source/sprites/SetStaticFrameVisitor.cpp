#include "SetStaticFrameVisitor.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "AnimSprite.h"
#include "SprVisitorParams.h"
#include "UpdateParams.h"
#include "Anim2Sprite.h"

namespace s2
{

SetStaticFrameVisitor::SetStaticFrameVisitor(int static_frame)
	: m_static_frame(static_frame)
{
}

VisitResult SetStaticFrameVisitor::Visit(const Sprite* spr, const SprVisitorParams& params)
{	
	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_ANCHOR) {
		return VISIT_INTO;
	}
	if (!spr->IsInheritUpdate()) {
		return VISIT_OVER;
	}

	VisitResult ret = VISIT_OVER;
	switch (type)
	{
	case SYM_ANIMATION:
		{
			AnimSprite* anim_spr = const_cast<AnimSprite*>(VI_DOWNCASTING<const AnimSprite*>(spr));
			anim_spr->SetActive(m_static_frame == -1, params.actor);
			anim_spr->SetFrame(UpdateParams(params.actor), m_static_frame);
		}
		break;
	case SYM_ANIM2:
		{
			Anim2Sprite* anim2_spr = const_cast<Anim2Sprite*>(VI_DOWNCASTING<const Anim2Sprite*>(spr));
			anim2_spr->SetStaticTime(s2::UpdateParams(), m_static_frame);
		}
		break;
	case SYM_COMPLEX: case SYM_MESH: case SYM_MASK: case SYM_PROXY:
		ret = VISIT_INTO;
		break;
    default:
        break;
	}
	return ret;
}

}