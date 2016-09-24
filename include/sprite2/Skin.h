#ifndef _SPRITE2_SKIN_H_
#define _SPRITE2_SKIN_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <string>
#include <vector>

namespace s2
{

class Skin : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Skin();
	~Skin();

private:
	

}; // Skin

}

#endif // _SPRITE2_SKIN_H_