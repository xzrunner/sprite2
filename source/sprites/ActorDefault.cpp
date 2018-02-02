#include "sprite2/ActorDefault.h"
#include "sprite2/CompActorTrans.h"

namespace s2
{

CU_SINGLETON_DEFINITION(ActorDefault);

ActorDefault::ActorDefault()
{
	m_transform = std::make_unique<CompActorTrans>();
}

}