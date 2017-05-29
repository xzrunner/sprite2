#include "AnimFlatten.h"

namespace s2
{

void AnimFlatten::UpdateTexcoords()
{
	for (int i = 0, n = m_frames.size(); i < n; ++i) {
		m_frames[i].UpdateTexcoords();
	}
}

void AnimFlatten::Clear()
{
	m_frames.clear();
}

RenderReturn AnimFlatten::Draw(const RenderParams& rp, int frame) const
{
	if (frame < 0 || frame >= m_frames.size()) {
		return RENDER_NO_DATA;
	}
	return m_frames[frame].Draw(rp);
}

}