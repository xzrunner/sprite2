#include "CameraMgr.h"

namespace s2
{

CU_SINGLETON_DEFINITION(CameraMgr)

CameraMgr::CameraMgr() 
	: m_type(ORTHO)
{
}

CameraMgr::~CameraMgr() 
{
}

}