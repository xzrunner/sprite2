#include "sprite2/ActorDefault.h"
#include "sprite2/ActorGeoTrans.h"

namespace s2
{

CU_SINGLETON_DEFINITION(ActorDefault);

ActorDefault::ActorDefault()
{
	m_geo = new ActorGeoTrans();
}

}