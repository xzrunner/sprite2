#include "sprite2/SprTimer.h"
#include "sprite2/Animation.h"
#include "sprite2/Animation2.h"
#include "sprite2/Particle2d.h"
#include "sprite2/Particle3d.h"
#include "sprite2/Trail.h"

#include "sprite2/UpdateParams.h"

namespace s2
{

CU_SINGLETON_DEFINITION(SprTimer)

SprTimer::SprTimer()
{
}

void SprTimer::Init()
{
	Animation::Instance();
	Animation2::Instance();
	Particle2d::Instance();
	Particle3d::Instance();
	Trail::Instance();
}

bool SprTimer::Update(float dt)
{
	bool ret = false;
	Animation::Instance()->Update(dt);
	Particle2d::Instance()->Update(dt);
	if (Particle3d::Instance()->BufferUpdate(dt)) {
		ret = true;
	}
	Trail::Instance()->Update(dt);
	return ret;
}

}