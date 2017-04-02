#include "SprTimer.h"
#include "Animation.h"
#include "Animation2.h"
#include "Particle2d.h"
#include "Particle3d.h"
#include "Trail.h"

#include "UpdateParams.h"

namespace s2
{

SINGLETON_DEFINITION(SprTimer)

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