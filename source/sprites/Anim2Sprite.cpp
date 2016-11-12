#include "Anim2Sprite.h"
#include "Anim2Symbol.h"

namespace s2
{

Anim2Sprite::Anim2Sprite() 
{
}

Anim2Sprite::Anim2Sprite(Anim2Symbol* sym) 
	: Sprite(sym)
	, m_curr(sym)
{
}

Anim2Sprite* Anim2Sprite::Clone() const
{
	return new Anim2Sprite(*this);
}

bool Anim2Sprite::Update(const RenderParams& params)
{
	return m_curr.Update();
}

}