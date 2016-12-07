#include "SprGeo.h"

#include <string.h>

namespace s2
{

// void SprGeo::SetWorldMat(const sm::mat4& mat)
// {
// 	m_world.m[0] = mat.x[0];
// 	m_world.m[1] = mat.x[1];
// 	m_world.m[2] = mat.x[4];
// 	m_world.m[3] = mat.x[5];
// 	m_world.m[4] = mat.x[12];
// 	m_world.m[5] = mat.x[13];
// }
// 
// sm::mat4 SprGeo::GetWorldMat()
// {
// 	sm::mat4 ret;
// 	ret.x[0] = m_world.m[0];
// 	ret.x[1] = m_world.m[1];
// 	ret.x[4] = m_world.m[2];
// 	ret.x[5] = m_world.m[3];
// 	ret.x[12]= m_world.m[4];
// 	ret.x[13]= m_world.m[5];
// 	return ret;
// }
// 	
/************************************************************************/
/* class SprGeo::SRT                                                    */
/************************************************************************/

SprGeo::SRT::SRT()
	: position(0, 0)
	, angle(0)
	, scale(1, 1)
	, shear(0, 0)
{
	offset.MakeInvalid();
}

// /************************************************************************/
// /* class SprGeo::Matrix                                                 */
// /************************************************************************/
// 
// SprGeo::Matrix::Matrix()
// {
// 	memset(m, 0, sizeof(m));
// 	m[0] = m[3] = 1;
// }

}