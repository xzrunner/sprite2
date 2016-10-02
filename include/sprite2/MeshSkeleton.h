#ifndef _SPRITE2_MESH_SKELETON_H_
#define _SPRITE2_MESH_SKELETON_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class MeshJoint;
class RenderParams;

class MeshSkeleton : public cu::RefCountObj, private cu::Uncopyable
{
public:
	MeshSkeleton();
	~MeshSkeleton();

	void Draw(const RenderParams& params) const;

	const MeshJoint* GetRoot() const { return m_root; }
	void SetRoot(const MeshJoint* joint);

	const MeshJoint* QueryByPos(const sm::vec2& pos) const;

protected:
	MeshJoint* m_root;

}; // MeshSkeleton

}

#endif // _SPRITE2_MESH_SKELETON_H_