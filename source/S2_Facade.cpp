#include "S2_Facade.h"
#include "SprLUT.h"
#include "Blackboard.h"

namespace s2
{

void Facade::Clear()
{
	SprLUT::Instance()->Clear();
	Blackboard::Instance()->Clear();
}

}