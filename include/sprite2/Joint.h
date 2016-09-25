#ifndef _SPRITE2_JOINT_H_
#define _SPRITE2_JOINT_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <string>
#include <vector>

namespace s2
{

class Skin;

class Joint : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Joint();
	~Joint();

	virtual void Update();
	virtual void Draw() const;

	bool Connect(Joint* joint);
	bool Deconnect();

private:
	std::string m_name;

	Joint* m_parent;
	std::vector<Joint*> m_children;

	Skin* m_skin;

}; // Joint

}

#endif // _SPRITE2_JOINT_H_