#include "MaskSprite.h"
#include "MaskSymbol.h"
#include "UpdateParams.h"
#include "S2_Actor.h"
#include "SprVisitorParams.h"

namespace s2
{

MaskSprite::MaskSprite() 
{
}

MaskSprite::MaskSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
}

MaskSprite* MaskSprite::Clone() const
{
	return new MaskSprite(*this);
}

void MaskSprite::OnMessage(const UpdateParams& up, Message msg)
{
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);

	UpdateParams up_child(up);
	up_child.Push(this);

	if (const Sprite* base = sym->GetBase()) {
		up_child.SetActor(base->QueryActor(up.GetActor()));
		const_cast<Sprite*>(base)->OnMessage(up_child, msg);
	}
	if (const Sprite* mask = sym->GetMask()) {
		up_child.SetActor(mask->QueryActor(up.GetActor()));
		const_cast<Sprite*>(mask)->OnMessage(up_child, msg);
	}
}

bool MaskSprite::Update(const UpdateParams& up)
{
	bool dirty = false;

	UpdateParams up_child(up);
	up_child.Push(this);

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		up_child.SetActor(base->QueryActor(up.GetActor()));
		if (const_cast<Sprite*>(base)->Update(up_child)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		up_child.SetActor(mask->QueryActor(up.GetActor()));
		if (const_cast<Sprite*>(mask)->Update(up_child)) {
			dirty = true;
		}
	}
	return dirty;
}

bool MaskSprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !IsInheritUpdate()) {
		return false;
	}

	bool dirty = false;

	UpdateParams up_child(up);
	up_child.Push(this);

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		up_child.SetActor(base->QueryActor(up.GetActor()));
		if (const_cast<Sprite*>(base)->SetFrame(up_child, frame)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		up_child.SetActor(mask->QueryActor(up.GetActor()));
		if (const_cast<Sprite*>(mask)->SetFrame(up_child, frame)) {
			dirty = true;
		}
	}

	return dirty;
}

Sprite* MaskSprite::FetchChild(const std::string& name, const Actor* actor) const
{
	if (name == "base") {
		return const_cast<Sprite*>(VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetBase());
	} else if (name == "mask") {
		return const_cast<Sprite*>(VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetMask());
	} else {
		return NULL;
	}
}

VisitResult MaskSprite::TraverseChildren(SpriteVisitor& visitor, SprVisitorParams& params) const
{
	const Sprite* mask = VI_DOWNCASTING<MaskSymbol*>(m_sym)->GetMask();
	SprVisitorParams cp = params;
	cp.actor = mask->QueryActor(params.actor);
	VisitResult ret = mask->TraverseChildren(visitor, cp);
	params.selected = cp.selected;
	return ret;
}

}