#include "AnimFlatten.h"

namespace s2
{

void AnimFlatten::Clear()
{
	m_frames.clear();
}

void AnimFlatten::Draw(const RenderParams& rp) const
{
	for (int i = 0, n = m_frames.size(); i < n; ++i) {
		m_frames[i].Draw(rp);
	}
}

}