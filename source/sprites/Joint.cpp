#include "Joint.h"
#include "Skin.h"

#include <algorithm>

namespace s2
{

Joint::Joint()
	: m_parent(NULL)
{
}

Joint::~Joint()
{
	if (m_parent) {
		m_parent->RemoveReference();
	}
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Joint>());
}

void Joint::Update()
{
	m_skin->Update();
}

void Joint::Draw() const
{
	m_skin->Draw();
}

bool Joint::Connect(Joint* joint)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == joint) {
			return false;
		}
	}
	m_children.push_back(joint);
	return true;
}

bool Joint::Deconnect()
{
	if (!m_parent) {
		return false;
	}
	for (int i = 0, n = m_parent->m_children.size(); i < n; ++i) {
		if (m_parent->m_children[i] == this) {
			m_parent->m_children.erase(m_parent->m_children.begin() + i);
			this->RemoveReference();
		}
	}
	m_parent->RemoveReference();
	m_parent = NULL;
	return true;
}

}