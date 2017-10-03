#include "sprite2/AnimState.h"
#include "sprite2/AnimCurr.h"
#include "sprite2/UpdateParams.h"

namespace s2
{

void AnimState::Assign(const AnimState& src, bool same_struct)
{
	assert(src.m_origin);
#ifdef S2_MULTITHREAD
	assert(src.m_update && src.m_draw);
#endif // S2_MULTITHREAD

	if (same_struct && IsVaild()) {
		m_origin->AssignSameStruct(*src.m_origin);
#ifdef S2_MULTITHREAD
		m_update->AssignSameStruct(*src.m_update);
		m_draw->AssignSameStruct(*src.m_draw);
#endif // S2_MULTITHREAD
	} else {
		m_origin = src.m_origin->Clone();
#ifdef S2_MULTITHREAD
		m_update = src.m_update->Clone();
		m_draw = src.m_draw->Clone();
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

void AnimState::Start(const SprConstPtr& spr)
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

void AnimState::Init(std::unique_ptr<AnimCurr>& dst, 
	                 const std::shared_ptr<AnimCopy>& copy)
{
	dst = std::make_unique<AnimCurr>();
	dst->SetAnimCopy(copy);
}

}