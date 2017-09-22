#include "ComplexFlatten.h"

namespace s2
{

void ComplexFlatten::UpdateTexcoords()
{
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		m_actions[i].UpdateTexcoords();
	}
}

void ComplexFlatten::Clear()
{
	m_actions.clear();
}

RenderReturn ComplexFlatten::Draw(const RenderParams& rp, int action) const
{
	const Flatten* flatten = GetFlatten(action);
	if (flatten) {
		return flatten->Draw(rp);
	} else {
		return RENDER_NO_DATA;
	}
}

bool ComplexFlatten::Update(const UpdateParams& up, int action)
{
	const Flatten* flatten = GetFlatten(action);
	if (flatten) {
		return const_cast<Flatten*>(flatten)->Update(up, NULL);
	} else {
		return false;
	}
}

const Flatten* ComplexFlatten::GetFlatten(int action) const
{
	if (action == -1 && !m_actions.empty()) {
		return &m_actions[0];
	} else {
		if (action >= 0 && action < static_cast<int>(m_actions.size())) {
			return &m_actions[action];
		} else {
			return NULL;
		}
	}
}

}