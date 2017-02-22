#include "MaskSprite.h"
#include "MaskSymbol.h"

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

void MaskSprite::OnMessage(Message msg)
{
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		const_cast<Sprite*>(base)->OnMessage(msg);
	}
	if (const Sprite* mask = sym->GetMask()) {
		const_cast<Sprite*>(mask)->OnMessage(msg);
	}
}

bool MaskSprite::Update(const RenderParams& params)
{
	bool dirty = false;
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		if (const_cast<Sprite*>(base)->Update(params)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		if (const_cast<Sprite*>(mask)->Update(params)) {
			dirty = true;
		}
	}
	return dirty;
}

bool MaskSprite::SetFrame(int frame)
{
	// todo
	return false;
}

Sprite* MaskSprite::FetchChild(const std::string& name) const
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