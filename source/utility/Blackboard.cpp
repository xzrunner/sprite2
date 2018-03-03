#include "sprite2/Blackboard.h"

#include <painting2/Camera.h>

namespace s2
{

CU_SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
	: m_screen_cache_texid(-1)
	, m_cam(nullptr)
	, m_dlist_enable(false)
{
}

void Blackboard::SetScreenSize(int w, int h) 
{ 
	if (m_cam) {
		m_cam->OnSize(w, h);
	}
}

void Blackboard::Clear()
{
	m_screen_cache_texid = -1;
	m_cam = nullptr;
}

}