#include "ComplexActor.h"
#include "ComplexSymbol.h"
#include "S2_Sprite.h"

namespace s2
{

ComplexActor::ComplexActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent)
	, m_action(-1)
{
	auto& scissor = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(spr->GetSymbol())->GetScissor();
	if (scissor.Width() > 0 && scissor.Height() > 0) {
		GetAABB().SetStaticRect(scissor);
	}
}

void ComplexActor::SetAction(int action) 
{ 
	if (m_action == action) {
		return;
	}

	m_action = action;

	GetAABB().Update(this);

	SetFlattenDirty();
}

int ComplexActor::GetAction() const 
{ 
	return m_action; 
}

}