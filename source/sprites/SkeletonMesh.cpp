#include "SkeletonMesh.h"
#include "PolylineShape.h"
#include "MeshSkeleton.h"

#include <SM_Triangulation.h>

namespace s2
{

SkeletonMesh::SkeletonMesh()
	: m_shape(NULL)
{
}

SkeletonMesh::SkeletonMesh(const SkeletonMesh& mesh)
	: Mesh(mesh)
{
	Init(mesh);
}

SkeletonMesh::SkeletonMesh(const Symbol* base)
	: Mesh(base)
	, m_shape(NULL)
{
}

SkeletonMesh& SkeletonMesh::operator = (const SkeletonMesh& mesh)
{
	Mesh::operator = (mesh);
	Init(mesh);
	return *this;
}

SkeletonMesh::~SkeletonMesh()
{
	if (m_shape) {
		delete m_shape;
	}
}

void SkeletonMesh::SetShape(PolylineShape* shape)
{
	cu::RefCountObjAssign(m_shape, shape);
	RefreshTriangles();
}

void SkeletonMesh::RefreshTriangles()
{
	if (!m_shape) {
		return;
	}

	ClearTriangles();

	std::vector<sm::vec2> tris;
	sm::triangulate_normal(m_shape->GetVertices(), tris);
	SetTriangles(tris);
}

void SkeletonMesh::Init(const SkeletonMesh& mesh)
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