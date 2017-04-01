#include "S2_Facade.h"
#include "SprLUT.h"

namespace s2
{

void Facade::Clear()
{
	SprLUT::Instance()->Clear();
}

}