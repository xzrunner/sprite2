#include "Anim2Sprite.h"
#include "Anim2Symbol.h"

namespace s2
{

Anim2Sprite::Anim2Sprite() 
{
}

Anim2Sprite::Anim2Sprite(Symbol* sym) 
	: Sprite(sym)
{
}

Anim2Sprite* Anim2Sprite::Clone() const
{
	return new Anim2Sprite(*this);
}

}