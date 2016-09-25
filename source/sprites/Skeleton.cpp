#include "Skeleton.h"
#include "Joint.h"

namespace s2
{

Skeleton::Skeleton()
{

}

Skeleton::~Skeleton()
{

}

void Skeleton::Update()
{

}

void Skeleton::Draw() const
{
	for (int i = 0, n = m_draw_order.size(); i < n; ++i) {
		m_draw_order[i]->Draw();
	}
}

}