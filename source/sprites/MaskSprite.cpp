#include "MaskSprite.h"
#include "MaskSymbol.h"
#include "SprTreePath.h"
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

void MaskSprite::OnMessage(Message msg, const SprTreePath& path)
{
	SprTreePath cpath = path;
	cpath.Push(*this);
	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		const_cast<Sprite*>(base)->OnMessage(msg, cpath);
	}
	if (const Sprite* mask = sym->GetMask()) {
		const_cast<Sprite*>(mask)->OnMessage(msg, cpath);
	}
}

bool MaskSprite::Update(const RenderParams& rp)
{
	bool dirty = false;

	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	rp_child.path.Push(*this);

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		if (const_cast<Sprite*>(base)->Update(rp_child)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		if (const_cast<Sprite*>(mask)->Update(rp_child)) {
			dirty = true;
		}
	}
	return dirty;
}

bool MaskSprite::SetFrame(int frame, const SprTreePath& path, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}

	bool dirty = false;

	SprTreePath cpath = path;
	cpath.Push(*this);

	MaskSymbol* sym = VI_DOWNCASTING<MaskSymbol*>(m_sym);
	if (const Sprite* base = sym->GetBase()) {
		if (const_cast<Sprite*>(base)->SetFrame(frame, cpath)) {
			dirty = true;
		}
	}
	if (const Sprite* mask = sym->GetMask()) {
		if (const_cast<Sprite*>(mask)->SetFrame(frame, cpath)) {
			dirty = true;
		}
	}

	return dirty;
}

Sprite* MaskSprite::FetchChild(const std::string& name, const SprTreePath& path) const
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