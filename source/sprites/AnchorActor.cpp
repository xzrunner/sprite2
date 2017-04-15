#include "AnchorActor.h"
#include "S2_Sprite.h"

#include <S2_Sprite.h>
#include <S2_Symbol.h>

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
			// disconnect
			const_cast<Actor*>(m_anchor)->SetParent(NULL);
			m_anchor->GetSpr()->RemoveReference();
		}
		m_anchor = anchor;
		if (anchor) {
			anchor->GetSpr()->AddReference();
		}
	}

	if (anchor) 
	{
		GetAABB().SetRect(anchor->GetAABB().GetRect());
		anchor->GetSpr()->ConnectActors(this);

		ActorAABB& aabb = const_cast<Actor*>(anchor)->GetAABB();
		aabb.SetRect(sm::rect());	// make it empty
		aabb.Update(this);
	} 
	else 
	{
		// make it empty
		GetAABB().SetRect(sm::rect());
	}
}

const Actor* AnchorActor::GetAnchor() const
{
	return m_anchor;
}

}