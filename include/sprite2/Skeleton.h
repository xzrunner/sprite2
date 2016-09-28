#ifndef _SPRITE2_SKELETON_H_
#define _SPRITE2_SKELETON_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <vector>

namespace s2
{

class Joint;
class RenderParams;

class Skeleton : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Skeleton(const Joint* root, const std::vector<Joint*>& all_joints);
	~Skeleton();

	void Draw(const RenderParams& params) const;

private:
	void Clear();

protected:
	const Joint* m_root;

	std::vector<Joint*> m_all_joints;

}; // Skeleton

}

#endif // _SPRITE2_SKELETON_H_