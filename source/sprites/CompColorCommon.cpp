#include "sprite2/CompColorCommon.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompColorCommon* CompColorCommon::Clone() const
{
	auto ret = static_cast<CompColorCommon*>(mm::AllocHelper::New<CompColorCommon>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

void CompColorCommon::Reset()
{
	m_col.Reset();
}

CompColorCommon* CompColorCommon::Create()
{
	return static_cast<CompColorCommon*>(mm::AllocHelper::New<CompColorCommon>());
}

}