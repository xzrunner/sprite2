#include "sprite2/Facade.h"
#include "sprite2/Blackboard.h"

namespace s2
{

void Facade::Clear()
{
	Blackboard::Instance()->Clear();
}

}