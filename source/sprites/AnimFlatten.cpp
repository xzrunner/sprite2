#include "AnimFlatten.h"

namespace s2
{

void AnimFlatten::Clear()
{
	m_frames.clear();
}

void AnimFlatten::Draw(const RenderParams& rp, int frame) const
{
	if (frame < 0 || frame >= m_frames.size()) {
		return;
	}
	m_frames[frame].Draw(rp);
}

}