#ifndef _SPRITE2_ILERP_H_
#define _SPRITE2_ILERP_H_

#include <memory>

#include <stddef.h>

namespace s2
{

class ILerp
{
public:
	virtual ~ILerp() {}

	virtual std::unique_ptr<ILerp> Clone() const { return nullptr; }

	virtual int Type() const = 0;

}; // ILerp

}

#endif // _SPRITE2_ILERP_H_