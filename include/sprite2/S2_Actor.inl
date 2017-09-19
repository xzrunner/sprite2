#ifndef _SPRITE2_ACTOR_INL_
#define _SPRITE2_ACTOR_INL_

namespace s2
{

inline
bool Actor::BuildFlatten()
{
	if (!m_flatten || IsFlattenDirty()) {
		m_flatten = std::make_unique<ft::Flatten>(this);
		return true;
	} else {
		return false;
	}
}

inline
bool Actor::HasFlatten() const 
{ 
	return m_flatten != nullptr; 
}

inline
void Actor::FlattenUpdate(bool force) 
{
	if (m_flatten) {
		m_flatten->Update(force);
	}
}

inline
void Actor::FlattenDraw(const s2::RenderParams& rp) const
{
	if (m_flatten) {
		m_flatten->Draw(rp);
	}
}

inline
void Actor::FlattenSetFrame(int frame)
{
	if (m_flatten) {
		m_flatten->SetFrame(false, frame);
	}
}

}

#endif // _SPRITE2_ACTOR_INL_