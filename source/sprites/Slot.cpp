//#include "Slot.h"
//#include "S2_Sprite.h"
//
//namespace s2
//{
//
//Slot::Slot()
//	: m_bone(NULL)
//{
//}
//
//Slot::Slot(const Slot& slot)
//	: m_name(slot.m_name)
//	, m_attachments(slot.m_attachments)
//	, m_bone(NULL)
//{
//	cu::RefCountObjAssign(m_bone, slot.m_bone);
//}
//
//Slot& Slot::operator = (const Slot& slot)
//{
//	m_name = slot.m_name;
//	m_attachments = slot.m_attachments;
//	m_bone = slot.m_bone;
//	if (m_bone) {
//		m_bone->AddReference();
//	}
//}
//
//Slot::~Slot()
//{
//	if (m_bone) {
//		m_bone->RemoveReference();
//	}
//}
//
//void Slot::Draw(const RenderParams& params) const
//{
//	
//}
//
///************************************************************************/
///* class Slot::Attachment                                               */
///************************************************************************/
//
//Slot::Attachment::
//Attachment() 
//	: spr(NULL)
//	, angle(0) 
//{}
//
//Slot::Attachment::
//~Attachment() 
//{
//	if (spr) {
//		spr->RemoveReference();
//	}
//}
//
//}