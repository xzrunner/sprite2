#include "Blackboard.h"

namespace s2
{

SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
	: m_screen_cache_texid(-1)
	, m_cam(NULL)
{
}

}