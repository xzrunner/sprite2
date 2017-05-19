#ifndef _SPRITE2_SPRITE_INL_
#define _SPRITE2_SPRITE_INL_

#include "SprNameMap.h"

namespace s2
{

inline
void Sprite::SetName(const std::string& name) 
{
	m_name = SprNameMap::Instance()->StrToID(name);	
	UpdateInheritUpdate();
}

inline
void Sprite::UpdateInheritUpdate() const
{
	if (m_name == -1) {
		SetInheritUpdate(false);
	} else {
		std::string name;
		SprNameMap::Instance()->IDToStr(m_name, name);
		SetInheritUpdate(IsForceUpdate() || name.empty() || name[0] == '_');
	}
}

inline
void Sprite::SetForceUpdate(bool flag) const
{
	if (flag) {
		m_flags |= FLAG_FORCE_UPDATE;
	} else {
		m_flags &= ~FLAG_FORCE_UPDATE;
	}
	UpdateInheritUpdate();
}

}

#endif // _SPRITE2_SPRITE_INL_