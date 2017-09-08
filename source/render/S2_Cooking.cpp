#include "sprite2/S2_Cooking.h"
#include "sprite2/RenderTask.h"
#include "sprite2/Callback.h"

namespace s2
{

SINGLETON_DEFINITION(Cooking);

Cooking::Cooking()
{
}

void Cooking::Draw(const Sprite* spr, RenderParams* rp)
{
	Callback::SubmitTask(RenderTask::Create(spr, rp));
}

}