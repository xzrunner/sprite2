#ifndef _SPRITE2_SKELETON_H_
#define _SPRITE2_SKELETON_H_

#include "RenderReturn.h"

#include <cu/uncopyable.h>
#include <SM_Rect.h>

#include <memory>
#include <vector>

namespace s2
{

class Joint;
class RenderParams;

class Skeleton : private cu::Uncopyable
{
public:
	Skeleton(const std::shared_ptr<Joint>& root, const std::vector<std::shared_ptr<Joint>>& all_joints);

	RenderReturn Draw(const RenderParams& rp) const;
	sm::rect GetBounding() const;

	std::shared_ptr<Joint> QueryByPos(const sm::vec2& pos) const;

	const std::vector<std::shared_ptr<Joint>>& GetAllJoints() const { return m_all_joints; }

private:
	void Clear();

protected:
	std::shared_ptr<const Joint> m_root;

	std::vector<std::shared_ptr<Joint>> m_all_joints;

}; // Skeleton

}

#endif // _SPRITE2_SKELETON_H_