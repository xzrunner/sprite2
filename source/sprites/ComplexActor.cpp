#include "ComplexActor.h"
#include "ComplexSymbol.h"
#include "S2_Sprite.h"

namespace s2
{

ComplexActor::ComplexActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_action(-1)
{
	const ComplexSymbol* comp_sym = VI_DOWNCASTING<const ComplexSymbol*>(spr->GetSymbol());
	const sm::rect& scissor = comp_sym->GetScissor();
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
}

int ComplexActor::GetAction() const 
{ 
	return m_action; 
}

}