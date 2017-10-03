#include "S2_Facade.h"
#include "Blackboard.h"

namespace s2
{

void Facade::Clear()
{
	Blackboard::Instance()->Clear();
}

}