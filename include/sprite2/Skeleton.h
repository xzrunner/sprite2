#ifndef _SPRITE2_SKELETON_H_
#define _SPRITE2_SKELETON_H_

#include "RenderReturn.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>
#include <SM_Rect.h>

#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class Joint;
class RenderParams;

class Skeleton : private cu::Uncopyable
{
public:
	Skeleton(const std::shared_ptr<Joint>& root, const CU_VEC<std::shared_ptr<Joint>>& all_joints);

	RenderReturn Draw(cooking::DisplayList* dlist, const RenderParams& rp) const;
	sm::rect GetBounding() const;

	std::shared_ptr<Joint> QueryByPos(const sm::vec2& pos) const;

	const CU_VEC<std::shared_ptr<Joint>>& GetAllJoints() const { return m_all_joints; }

private:
	void Clear();

protected:
	std::shared_ptr<const Joint> m_root;

	CU_VEC<std::shared_ptr<Joint>> m_all_joints;

}; // Skeleton

}

#endif // _SPRITE2_SKELETON_H_