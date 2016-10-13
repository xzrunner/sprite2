#include "NetworkShape.h"

namespace s2
{

NetworkShape::NetworkShape()
	: PolylineShape()
{
	m_closed = true;
}

NetworkShape::NetworkShape(const NetworkShape& nw)
	: PolylineShape(nw)
{
	SetInnerVertices(nw.m_inner_vertices);
}

NetworkShape::NetworkShape(const std::vector<sm::vec2>& vertices)
	: PolylineShape(vertices, true)
{
}

NetworkShape* NetworkShape::Clone() const
{
	return new NetworkShape(*this);
}

void NetworkShape::SetInnerVertices(const std::vector<sm::vec2>& inner) 
{
	m_inner_vertices.clear();
	for (int i = 0, n = inner.size(); i < n; ++i) {
		bool find = false;
		for (int j = 0, m = m_vertices.size(); j < m; ++j) {
			if (inner[i] == m_vertices[j]) {
				find = true;
				break;
			}
		}
		if (!find) {
			m_inner_vertices.push_back(inner[i]);
		}
	}
}

}