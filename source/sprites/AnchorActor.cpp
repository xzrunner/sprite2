#include "sprite2/AnchorActor.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"

namespace s2
{

void AnchorActor::SetAnchor(const ActorPtr& anchor)
{
	// anchor mount itself mean clean
	if (anchor.get() == this) {
		Clear();
		return;
	}

	if (m_anchor.actor != anchor)
	{
		// disconnect
		if (m_anchor.actor) {
			m_anchor.actor->SetParent(nullptr);
		}
		m_anchor.actor = anchor;
		// cache spr
		if (anchor) {
			m_anchor.spr = std::const_pointer_cast<Sprite>(anchor->GetSpr());
		}
	}

	if (anchor) 
	{
		GetAABB().SetRect(anchor->GetAABB().GetRect());
		anchor->GetSprRaw()->ConnectActors(shared_from_this());

		auto parent = anchor->GetParent();
		if (parent) {
			const_cast<ActorAABB&>(parent->GetAABB()).SetRect(sm::rect());	// make it empty
		}
		anchor->GetAABB().UpdateParent(anchor.get());
	} 
	else 
	{
		GetAABB().Update(this);
	}
}

void AnchorActor::Clear()
{
	if (m_anchor.actor) 
	{
		// disconnect
		m_anchor.actor->SetParent(nullptr);
		m_anchor.actor.reset();
		m_anchor.spr.reset();
	}

	// make it empty
	GetAABB().SetRect(sm::rect());
}

}