#include "SkeletonMesh.h"
#include "PolylineShape.h"
#include "MeshSkeleton.h"
#include "MeshNode.h"
#include "MeshTriangle.h"
#include "MeshJoint.h"

#include <SM_Triangulation.h>
#include <SM_Calc.h>

#include <set>
#include <queue>

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

	BindMeshNode();
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

void SkeletonMesh::BindMeshNode()
{
	if (!m_skeleton.GetRoot()) {
		return;
	}

	std::set<MeshNode*> unique;
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			MeshNode* node = m_tris[i]->nodes[j];
			node->xy = node->ori_xy;
			unique.insert(node);
		}
	}

	for (std::set<MeshNode*>::iterator itr = unique.begin(); itr != unique.end(); ++itr) {
		BindMeshNode(*itr);
	}
}

void SkeletonMesh::BindMeshNode(MeshNode* node)
{
	s2::MeshJoint* bind = NULL;
	float nearest = FLT_MAX;
	const MeshJoint* root = m_skeleton.GetRoot();
	std::queue<const MeshJoint*> buf;
	buf.push(root);
	while (!buf.empty())
	{
		const MeshJoint* parent = buf.front(); buf.pop();
		const std::vector<s2::MeshJoint*>& children = parent->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			s2::MeshJoint* child = children[i];
			float dis = sm::dis_pos_to_seg(node->ori_xy, parent->GetWorldPose().trans, child->GetWorldPose().trans);
			if (dis < nearest) {
				nearest = dis;
				bind = child;
			}
			buf.push(child);
		}
	}

	if (bind) {
		bind->BindNode(node, 1);
	}
}

}