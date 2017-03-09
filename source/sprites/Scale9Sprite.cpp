#include "Scale9Sprite.h"
#include "Scale9Symbol.h"
#include "SprVisitor.h"

#include <stddef.h>

namespace s2
{

Scale9Sprite::Scale9Sprite() 
{
}

Scale9Sprite::Scale9Sprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_s9(VI_DOWNCASTING<Scale9Symbol*>(sym)->GetScale9())
{
}

Scale9Sprite* Scale9Sprite::Clone() const
{
	return new Scale9Sprite(*this);
}

bool Scale9Sprite::TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const
{
	std::vector<Sprite*> grids;
	m_s9.GetGrids(grids);
	if (visitor.GetOrder()) {
		for (int i = 0, n = grids.size(); i < n; ++i) {
			if (!grids[i]->Traverse(visitor, params)) {
				return false;
			}
		}
	} else {
		for (int i = grids.size() - 1; i >= 0; --i) {
			if (!grids[i]->Traverse(visitor, params)) {
				return false;
			}
		}
	}
	return true;
}

void Scale9Sprite::Resize(float width, float height)
{
	m_s9.SetSize(width, height);
}

}