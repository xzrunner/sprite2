#include "sprite2/CompColorMap.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompColorMap::~CompColorMap()
{
	mm::AllocHelper::Delete(this);
}

CompColorMap* CompColorMap::Clone() const
{
	auto ret = static_cast<CompColorMap*>(mm::AllocHelper::New<CompColorMap>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

void CompColorMap::Reset()
{
	m_col.Reset();
}

CompColorMap* CompColorMap::Create()
{
	return static_cast<CompColorMap*>(mm::AllocHelper::New<CompColorMap>());
}

}