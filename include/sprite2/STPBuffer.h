#ifndef _SPRITE2_SPT_BUFFER_H_
#define _SPRITE2_SPT_BUFFER_H_

#include <stdint.h>

namespace s2
{

struct STPBuffer
{
	uint16_t cap;
	uint16_t num;
	int*     ids;

	STPBuffer* next;

	STPBuffer(int cap);
	~STPBuffer();

	bool CopyFrom(const STPBuffer* buf);

}; // STPBuffer

}

#endif // _SPRITE2_SPT_BUFFER_H_