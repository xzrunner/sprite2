#include "StatDrawCall.h"

#include <string.h>

namespace s2
{

SINGLETON_DEFINITION(StatDrawCall);

StatDrawCall::StatDrawCall()
{
	Reset();
}

void StatDrawCall::Print(std::string& str) const
{
	static char buf[512];
	sprintf(buf, "DC: scissor %d", m_scissor);
	str += buf;
}

void StatDrawCall::Reset()
{
	memset(this, 0, sizeof(*this));
}

}