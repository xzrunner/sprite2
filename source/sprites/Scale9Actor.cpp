#include "Scale9Actor.h"
#include "Scale9Sprite.h"

namespace s2
{

Scale9Actor::Scale9Actor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent)
	, m_s9(VI_DOWNCASTING<const Scale9Sprite*>(spr)->GetScale9())
{
}

void Scale9Actor::Resize(float width, float height)
{
	sm::vec2 sz = m_s9.GetSize();
	if (width == sz.x && height == sz.y) {
		return;
	}
	m_s9.SetSize(width, height);
	SetFlattenDirtyToRoot();
}

}