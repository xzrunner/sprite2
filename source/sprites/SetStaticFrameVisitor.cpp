#include "sprite2/SetStaticFrameVisitor.h"
#include "sprite2/SymType.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/AnimSprite.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/Anim2Sprite.h"
#include "sprite2/AudioSprite.h"

namespace s2
{

static const float FPS = 30.0f;

SetStaticFrameVisitor::SetStaticFrameVisitor(int static_frame)
	: m_static_frame(static_frame)
{
}

VisitResult SetStaticFrameVisitor::Visit(const SprConstPtr& spr, const SprVisitorParams& params)
{	
	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_ANCHOR) {
		return VISIT_INTO;
	}

	// update inherit
	if (!spr->IsInheritUpdate()) {
		return VISIT_OVER;
	}

	// visible
	auto& actor = params.actor;
	bool visible = actor ? actor->IsVisible() : spr->IsVisible();
	if (!visible) {
		return VISIT_OVER;
	}

	VisitResult ret = VISIT_OVER;
	switch (type)
	{
	case SYM_ANIMATION:
		{
			auto anim_spr = std::const_pointer_cast<AnimSprite>(S2_VI_PTR_DOWN_CAST<const AnimSprite>(spr));
			anim_spr->SetFrame(UpdateParams(actor), m_static_frame);
		}
		break;
	case SYM_ANIM2:
		{
			auto anim2_spr = S2_VI_PTR_DOWN_CAST<const Anim2Sprite>(spr);
			if (anim2_spr->GetStaticTime() != m_static_frame) {
				std::const_pointer_cast<Anim2Sprite>(anim2_spr)->SetStaticTime(UpdateParams(), m_static_frame);
			}
		}
		break;
	case SYM_COMPLEX: case SYM_MESH: case SYM_MASK: case SYM_PROXY:
		ret = VISIT_INTO;
		break;
	case SYM_AUDIO:
		{
			auto audio_spr = S2_VI_PTR_DOWN_CAST<const AudioSprite>(spr);
			std::const_pointer_cast<AudioSprite>(audio_spr)->Seek(m_static_frame / FPS);
		}
		break;
    default:
        break;
	}
	return ret;
}

}