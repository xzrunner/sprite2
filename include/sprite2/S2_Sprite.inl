#ifndef _SPRITE2_SPRITE_INL_
#define _SPRITE2_SPRITE_INL_

namespace s2
{

inline
void Sprite::SetName(const std::string& name) 
{ 
	m_name = name;
	UpdateInheritUpdate();
}

inline
void Sprite::UpdateInheritUpdate() const
{
	SetInheritUpdate(IsForceUpdate() || m_name.empty() || m_name[0] == '_');
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