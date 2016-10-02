#include "NetworkMesh.h"
#include "NetworkShape.h"
#include "MeshTriangle.h"

#include <SM_Triangulation.h>

#include <map>

#include <assert.h>

namespace s2
{

NetworkMesh::NetworkMesh()
	: m_shape(NULL)
{
}

NetworkMesh::NetworkMesh(const NetworkMesh& mesh)
	: Mesh(mesh)
{
	Init(mesh);
}

NetworkMesh::NetworkMesh(const Symbol* base)
	: Mesh(base)
	, m_shape(NULL)
{
}

NetworkMesh& NetworkMesh::operator = (const NetworkMesh& mesh)
{
	Mesh::operator = (mesh);
	Init(mesh);
	return *this;
}

NetworkMesh::~NetworkMesh()
{
	if (m_shape) {
		delete m_shape;
	}
}

void NetworkMesh::SetShape(NetworkShape* shape)
{
	cu::RefCountObjAssign(m_shape, shape);
	RefreshTriangles();
}

void NetworkMesh::RefreshTriangles()
{
	if (!m_shape) {
		return;
	}

	ClearTriangles();

	std::vector<sm::vec2> tris;
	sm::triangulate_points(m_shape->GetVertices(), m_shape->GetInnerVertices(), tris);
	SetTriangles(tris);
}

//void NetworkMesh::GetRegionBound(std::vector<sm::vec2>& bound) const
//{
// 	if (m_use_region) {
// 		const sm::rect& r = m_region.rect;
// 		bound.push_back(sm::vec2(r.xmin, r.ymin));
// 		bound.push_back(sm::vec2(r.xmin, r.ymax));
// 		bound.push_back(sm::vec2(r.xmax, r.ymax));
// 		bound.push_back(sm::vec2(r.xmax, r.ymin));
// 	} else {
// 		std::copy(m_region.nodes.begin(), m_region.nodes.end(), back_inserter(bound));
// 	}
//}

void NetworkMesh::Init(const NetworkMesh& mesh)
{
	if (mesh.m_shape) {
		m_shape = mesh.m_shape->Clone();
	} else {
		m_shape = NULL;
	}

	RefreshTriangles();
	CopyTriangles(mesh);
}

}