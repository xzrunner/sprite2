#ifndef _SPRITE2_ILERP_H_
#define _SPRITE2_ILERP_H_

#include <CU_Cloneable.h>

#include <stddef.h>

namespace s2
{

class ILerp : public cu::Cloneable
{
public:
	virtual ~ILerp() {}

	virtual ILerp* Clone() const { return NULL; }

	virtual int Type() const = 0;

}; // ILerp

}

#endif // _SPRITE2_ILERP_H_