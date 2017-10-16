#include "ActorDefault.h"
#include "ActorGeo.h"

namespace s2
{

CU_SINGLETON_DEFINITION(ActorDefault);

ActorDefault::ActorDefault()
{
	m_geo = new ActorGeo();
}

}