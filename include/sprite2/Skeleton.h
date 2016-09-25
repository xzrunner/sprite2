#ifndef _SPRITE2_SKELETON_H_
#define _SPRITE2_SKELETON_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <vector>

namespace s2
{

class Joint;

class Skeleton : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Skeleton();
	~Skeleton();

	void Update();
	void Draw() const;

protected:
	Joint* m_root;

	std::vector<Joint*> m_draw_order;

}; // Skeleton

}

#endif // _SPRITE2_SKELETON_H_