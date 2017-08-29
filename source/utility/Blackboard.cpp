#include "Blackboard.h"
#include "Camera.h"

namespace s2
{

SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
	: m_screen_cache_texid(-1)
	, m_cam(NULL)
{
}

void Blackboard::SetScreenSize(int w, int h) 
{ 
	m_screen_sz.x = w; 
	m_screen_sz.y = h; 

	if (m_cam) {
		m_cam->OnSize(w, h);
	}
}

void Blackboard::Clear()
{
	m_screen_cache_texid = -1;
	m_cam = NULL;
	m_screen_sz.Set(0, 0);
}

}