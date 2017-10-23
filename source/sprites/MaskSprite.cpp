#include "MaskSprite.h"
#include "MaskSymbol.h"
#include "UpdateParams.h"
#include "S2_Actor.h"
#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

namespace s2
{

MaskSprite::MaskSprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

MaskSprite::MaskSprite(const MaskSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

MaskSprite& MaskSprite::operator = (const MaskSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

MaskSprite::MaskSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

MaskSprite::~MaskSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_MASK);
#endif // S2_DISABLE_STATISTICS
}

void MaskSprite::OnMessage(const UpdateParams& up, Message msg)
{
	auto sym = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym);

	UpdateParams up_child(up);
	up_child.Push(this);

	if (auto& base = sym->GetBase()) {
		up_child.SetActor(base->QueryActor(up.GetActor()));
		std::const_pointer_cast<Sprite>(base)->OnMessage(up_child, msg);
	}
	if (auto& mask = sym->GetMask()) {
		up_child.SetActor(mask->QueryActor(up.GetActor()));
		std::const_pointer_cast<Sprite>(mask)->OnMessage(up_child, msg);
	}
}

bool MaskSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	bool dirty = false;

	UpdateParams up_child(up);
	up_child.Push(this);

	auto sym = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym);
	if (auto& base = sym->GetBase()) {
		up_child.SetActor(base->QueryActor(up.GetActor()));
		if (std::const_pointer_cast<Sprite>(base)->Update(up_child)) {
			dirty = true;
		}
	}
	if (auto& mask = sym->GetMask()) {
		up_child.SetActor(mask->QueryActor(up.GetActor()));
		if (std::const_pointer_cast<Sprite>(mask)->Update(up_child)) {
			dirty = true;
		}
	}

	return dirty;
}

SprPtr MaskSprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	SprPtr ret = nullptr;
	if (name == SprNameMap::BASE_ID) {
		ret = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym)->GetBase();
	} else if (name == SprNameMap::MASK_ID) {
		ret = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym)->GetMask();
	}
	return ret;
}

VisitResult MaskSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	auto& base = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym)->GetBase();
	if (base) {
		SprVisitorParams cp = params;
		cp.actor = base->QueryActor(params.actor);
		if (!SpriteVisitor::VisitChild(visitor, cp, base, ret))
			return ret;
	}
	auto& mask = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym)->GetMask();
	if (mask) {
		SprVisitorParams cp = params;
		cp.actor = mask->QueryActor(params.actor);
		if (!SpriteVisitor::VisitChild(visitor, cp, mask, ret))
			return ret;
	}
	return ret;
}

VisitResult MaskSprite::TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const
{
	VisitResult ret = VISIT_OVER;
	auto& base = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym)->GetBase();
	if (base) {
		SprVisitorParams2 cp = params;
		cp.actor = base->QueryActorRef(params.actor.get());
		if (!SpriteVisitor2::VisitChild(visitor, cp, base, ret))
			return ret;
	}
	auto& mask = S2_VI_PTR_DOWN_CAST<MaskSymbol>(m_sym)->GetMask();
	if (mask) {
		SprVisitorParams2 cp = params;
		cp.actor = mask->QueryActorRef(params.actor.get());
		if (!SpriteVisitor2::VisitChild(visitor, cp, mask, ret))
			return ret;
	}
	return ret;
}

}
