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

MaskSprite::MaskSprite(Symbol* sym, uint32_t id)
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

MaskSprite* MaskSprite::Clone() const
{
	return new MaskSprite(*this);
}

void MaskSprite::OnMessage(const UpdateParams& up, Message msg)
{
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(this);

	if (const Sprite* base = sym->GetBase()) {
		up_child->SetActor(base->QueryActor(up.GetActor()));
		const_cast<Sprite*>(base)->OnMessage(*up_child, msg);
	}
	if (const Sprite* mask = sym->GetMask()) {
		up_child->SetActor(mask->QueryActor(up.GetActor()));
		const_cast<Sprite*>(mask)->OnMessage(*up_child, msg);
	}

	UpdateParamsPool::Instance()->Push(up_child); 
}

bool MaskSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	const Actor* actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	bool dirty = false;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(this);

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		up_child->SetActor(base->QueryActor(up.GetActor()));
		if (const_cast<Sprite*>(base)->Update(*up_child)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		up_child->SetActor(mask->QueryActor(up.GetActor()));
		if (const_cast<Sprite*>(mask)->Update(*up_child)) {
			dirty = true;
		}
	}

	UpdateParamsPool::Instance()->Push(up_child); 

	return dirty;
}

Sprite* MaskSprite::FetchChildByName(int name, const Actor* actor) const
{
	Sprite* ret = nullptr;
	if (name == SprNameMap::BASE_ID) {
		ret = const_cast<Sprite*>(VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetBase());
	} else if (name == SprNameMap::MASK_ID) {
		ret = const_cast<Sprite*>(VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetMask());
	}
	if (ret) {
		ret->AddReference();
	}
	return ret;
}

VisitResult MaskSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	Sprite* base = const_cast<Sprite*>(VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetBase());
	if (base) {
		SprVisitorParams cp = params;
		cp.actor = base->QueryActor(params.actor);
		if (!SpriteVisitor::VisitChild(visitor, cp, base, ret))
			return ret;
	}
	Sprite* mask = const_cast<Sprite*>(VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetMask());
	if (mask) {
		SprVisitorParams cp = params;
		cp.actor = mask->QueryActor(params.actor);
		if (!SpriteVisitor::VisitChild(visitor, cp, mask, ret))
			return ret;
	}
	return ret;
}

}
