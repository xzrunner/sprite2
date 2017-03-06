#include "S2_Facade.h"
#include "SprLUT.h"
#include "ActorLUT.h"

namespace s2
{

void Facade::Clear()
{
	ActorLUT::Instance()->Clear();
	SprLUT::Instance()->Clear();
}

}