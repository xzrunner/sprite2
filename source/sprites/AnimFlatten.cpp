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
	if (frame >= 0 && frame < m_frames.size()) {
		return m_frames[frame].Draw(rp);
	} else {
		return RENDER_NO_DATA;
	}
}

bool AnimFlatten::Update(const UpdateParams& up, int frame)
{
	if (frame >= 0 && frame < m_frames.size()) {
		return m_frames[frame].Update(up, NULL);
	} else {
		return false;
	}
}

void AnimFlatten::SetFrame(const UpdateParams& up, int frame)
{
	if (frame >= 0 && frame < m_frames.size()) {
		m_frames[frame].SetFrame(up, frame);
	}
}

}