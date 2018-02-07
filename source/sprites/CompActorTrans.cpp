#include "sprite2/CompActorTrans.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompActorTrans* CompActorTrans::Clone() const
{
	auto ret = static_cast<CompActorTrans*>(mm::AllocHelper::New<CompActorTrans>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

CompActorTrans* CompActorTrans::Create()
{
	auto ret = static_cast<CompActorTrans*>(mm::AllocHelper::New<CompActorTrans>());
	ret->m_transform.Reset();
	return ret;
}

}