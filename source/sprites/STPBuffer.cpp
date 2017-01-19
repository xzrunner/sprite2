#include "STPBuffer.h"

namespace s2
{

STPBuffer::STPBuffer(int cap)
	: cap(cap)
	, num(0)
	, next(0)
{
	ids = new int[cap];
}

STPBuffer::~STPBuffer()
{
	delete[] ids;
}

bool STPBuffer::CopyFrom(const STPBuffer* buf)
{
	if (cap < buf->num) {
		return false;
	}
	
	num = buf->num;
	memcpy(ids, buf->ids, sizeof(int) * num);

	return true;
}

}