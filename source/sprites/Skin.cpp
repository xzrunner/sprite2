#include "Skin.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

namespace s2
{

Skin::Skin()
	: m_spr(NULL)
	, m_angle(0)
{
}

Skin::~Skin()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void Skin::Update()
{

}

void Skin::Draw() const
{
	DrawNode::Draw(m_spr);
}

}