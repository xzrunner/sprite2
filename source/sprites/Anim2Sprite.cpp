#include "Anim2Sprite.h"
#include "Anim2Symbol.h"

namespace s2
{

Anim2Sprite::Anim2Sprite() 
	: m_static_time(-1)
{
}

Anim2Sprite::Anim2Sprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_curr(VI_DOWNCASTING<Anim2Symbol*>(sym))
	, m_static_time(-1)
{
}

Anim2Sprite* Anim2Sprite::Clone() const
{
	return new Anim2Sprite(*this);
}

bool Anim2Sprite::Update(const UpdateParams& up)
{
	bool ret = m_curr.Update();
	if (ret) {
		SetBoundingDirty(true);
	}
	return ret;
}

bool Anim2Sprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}
	bool ret = m_curr.Update();
	if (ret) {
		SetBoundingDirty(true);
	}
	return ret;
}

void Anim2Sprite::SetStaticTime(int static_time)
{
	if (m_static_time == static_time) {
		return;
	}

	m_static_time = static_time;
	m_curr.SetStaticCurrFrame(static_time);
}

}