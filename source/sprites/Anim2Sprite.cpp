#include "Anim2Sprite.h"
#include "Anim2Symbol.h"
#include "UpdateParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

namespace s2
{

Anim2Sprite::Anim2Sprite() 
	: m_static_time(-1)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIM2);
#endif // S2_DISABLE_STATISTICS
}

Anim2Sprite::Anim2Sprite(const Anim2Sprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIM2);
#endif // S2_DISABLE_STATISTICS
}

Anim2Sprite& Anim2Sprite::operator = (const Anim2Sprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

Anim2Sprite::Anim2Sprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_curr(S2_VI_PTR_DOWN_CAST<Anim2Symbol>(sym))
	, m_static_time(-1)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIM2);
#endif // S2_DISABLE_STATISTICS
}

Anim2Sprite::~Anim2Sprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_ANIM2);
#endif // S2_DISABLE_STATISTICS
}

bool Anim2Sprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto& actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	bool ret = m_curr.Update();
	if (ret) {
		SetBoundingDirty(true);
	}
	return ret;
}

bool Anim2Sprite::AutoUpdate(const Actor* actor)
{
	bool ret = m_curr.Update();
	if (ret) {
		SetBoundingDirty(true);
	}
	return ret;
}

void Anim2Sprite::SetStaticTime(const UpdateParams& up, int static_time)
{
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}
	if (m_static_time == static_time) {
		return;
	}
	m_static_time = static_time;
	m_curr.SetStaticCurrFrame(static_time);
}

}