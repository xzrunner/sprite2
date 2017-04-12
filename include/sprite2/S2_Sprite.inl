#ifndef _SPRITE2_SPRITE_INL_
#define _SPRITE2_SPRITE_INL_

namespace s2
{

inline
const std::string& Sprite::GetName() const 
{ 
	return m_name; 
}

inline
void Sprite::SetName(const std::string& name) 
{ 
	m_name = name;
	
	SetInheritUpdate(IsForceUpdate() || name.empty() || name[0] == '_');
}

}

#endif // _SPRITE2_SPRITE_INL_