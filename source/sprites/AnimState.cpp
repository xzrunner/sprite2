#include "sprite2/AnimState.h"
#include "sprite2/AnimCurr.h"
#include "sprite2/UpdateParams.h"

namespace s2
{

#ifdef S2_MULTITHREAD
AnimState::AnimState()
	: m_origin(mm::allocate_unique<AnimCurr>()),
	, m_origin(mm::allocate_unique<AnimCurr>()),
	, m_origin(mm::allocate_unique<AnimCurr>()),
{
}
#else
AnimState::AnimState()
	: m_origin(mm::allocate_unique<AnimCurr>())
{
}
#endif // S2_MULTITHREAD

void AnimState::Assign(const AnimState& src, bool same_struct)
{
	assert(src.m_origin);
#ifdef S2_MULTITHREAD
	assert(src.m_update && src.m_draw);
#endif // S2_MULTITHREAD

	if (same_struct) {
		m_origin->AssignSameStruct(*src.m_origin);
#ifdef S2_MULTITHREAD
		m_update->AssignSameStruct(*src.m_update);
		m_draw->AssignSameStruct(*src.m_draw);
#endif // S2_MULTITHREAD
	} else {
		m_origin = mm::allocate_unique<AnimCurr>(*src.m_origin);;
#ifdef S2_MULTITHREAD
		m_update = mm::allocate_unique<AnimCurr>(*src.m_update);
		m_draw = mm::allocate_unique<AnimCurr>(*src.m_draw);
#endif // S2_MULTITHREAD
	}
}

void AnimState::Init(const std::shared_ptr<AnimCopy>& copy)
{
	Init(m_origin, copy);
#ifdef S2_MULTITHREAD
	Init(m_update, copy);
	Init(m_draw, copy);
#endif // S2_MULTITHREAD
}

void AnimState::Start(const Sprite* spr)
{
	UpdateParams up;
	m_origin->Start(up, spr);
#ifdef S2_MULTITHREAD
	m_update->Start(up, spr);
	m_draw->Start(up, spr);
#endif // S2_MULTITHREAD
}

#ifdef S2_MULTITHREAD
void AnimState::Flush()
{
	m_draw->AssignSameStruct(*m_update);
	m_update->AssignSameStruct(*m_origin);
}
#endif // S2_MULTITHREAD

void AnimState::Init(AnimCurrPtr& dst, const std::shared_ptr<AnimCopy>& copy)
{
	dst = mm::allocate_unique<AnimCurr>();
	dst->SetAnimCopy(copy);
}

}