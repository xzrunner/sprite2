#include "AnchorActor.h"
#include "S2_Sprite.h"

namespace s2
{

AnchorActor::AnchorActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_anchor(NULL)
{
}

AnchorActor::~AnchorActor() 
{
	if (m_anchor) {
		m_anchor->GetSpr()->RemoveReference();
	}
}

void AnchorActor::SetAnchor(const Actor* anchor) 
{
	if (m_anchor != anchor) {
		if (m_anchor) {
			m_anchor->GetSpr()->RemoveReference();
		}
		m_anchor = anchor;
		if (anchor) {
			anchor->GetSpr()->AddReference();
		}
	}

	if (anchor) {
		anchor->GetSpr()->ConnectActors(this);
	}
}

const Actor* AnchorActor::GetAnchor() const
{
	return m_anchor;
}

}