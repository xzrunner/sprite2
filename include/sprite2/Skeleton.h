#ifndef _SPRITE2_SKELETON_H_
#define _SPRITE2_SKELETON_H_

namespace s2
{

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	void Update();
	void Draw() const;

private:
	Joint* m_root;

	std::vector<Joint*> m_draw_order;

}; // Skeleton

}

#endif // _SPRITE2_SKELETON_H_