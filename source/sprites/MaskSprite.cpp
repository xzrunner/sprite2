#include "MaskSprite.h"
#include "MaskSymbol.h"
#include "RenderParams.h"

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

void MaskSprite::OnMessage(Message msg, const Actor* actor)
{
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		const_cast<Sprite*>(base)->OnMessage(msg, base->QueryActor(actor));
	}
	if (const Sprite* mask = sym->GetMask()) {
		const_cast<Sprite*>(mask)->OnMessage(msg, mask->QueryActor(actor));
	}
}

bool MaskSprite::Update(const RenderParams& rp)
{
	bool dirty = false;

	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		rp_child.actor = base->QueryActor(rp.actor);
		if (const_cast<Sprite*>(base)->Update(rp_child)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		rp_child.actor = mask->QueryActor(rp.actor);
		if (const_cast<Sprite*>(mask)->Update(rp_child)) {
			dirty = true;
		}
	}
	return dirty;
}

bool MaskSprite::SetFrame(int frame, const Actor* actor, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}

	bool dirty = false;

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		if (const_cast<Sprite*>(base)->SetFrame(frame, base->QueryActor(actor))) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		if (const_cast<Sprite*>(mask)->SetFrame(frame, mask->QueryActor(actor))) {
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

}