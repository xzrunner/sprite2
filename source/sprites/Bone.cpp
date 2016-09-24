//#include "Bone.h"
//#include "Skin.h"
//
//#include <algorithm>
//
//namespace s2
//{
//
//Bone::Bone()
//	: m_parent(NULL)
//	, m_length(0)
//	, m_angle(0)
//	, m_skin(-1)
//{
//}
//
//Bone::~Bone()
//{
//	if (m_parent) {
//		m_parent->RemoveReference();
//	}
//	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Bone>());
//	for_each(m_skins.begin(), m_skins.end(), cu::RemoveRefFonctor<Skin>());
//}
//
//}