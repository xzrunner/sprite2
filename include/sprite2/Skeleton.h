#ifndef _SPRITE2_SKELETON_H_
#define _SPRITE2_SKELETON_H_

#include "RenderReturn.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <SM_Rect.h>

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

	RenderReturn Draw(const RenderParams& rp) const;
	sm::rect GetBounding() const;

	const Joint* QueryByPos(const sm::vec2& pos) const;

	const std::vector<Joint*>& GetAllJoints() const { return m_all_joints; }

private:
	void Clear();

protected:
	const Joint* m_root;

	std::vector<Joint*> m_all_joints;

}; // Skeleton

}

#endif // _SPRITE2_SKELETON_H_