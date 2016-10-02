#ifndef _SPRITE2_SKELETON_MESH_H_
#define _SPRITE2_SKELETON_MESH_H_

#include "Mesh.h"
#include "MeshSkeleton.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class PolylineShape;

class SkeletonMesh : public VIRTUAL_INHERITANCE Mesh
{
public:
	SkeletonMesh();
	SkeletonMesh(const SkeletonMesh& mesh);
	SkeletonMesh(const Symbol* base);
	SkeletonMesh& operator = (const SkeletonMesh& mesh);
	virtual ~SkeletonMesh();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual SkeletonMesh* Clone() const { return new SkeletonMesh(*this); }	
	
	void SetShape(PolylineShape* shape);
	const PolylineShape* GetShape() const { return m_shape; }

	MeshSkeleton& GetSkeleton() { return m_skeleton; }

protected:
	void RefreshTriangles();

private:
	void Init(const SkeletonMesh& mesh);

protected:
	PolylineShape* m_shape;

	MeshSkeleton m_skeleton;
	
	VI_DUMMY_FUNC

}; // SkeletonMesh

}

#endif // _SPRITE2_SKELETON_MESH_H_