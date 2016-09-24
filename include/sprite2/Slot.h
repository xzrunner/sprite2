//#ifndef _SPRITE2_SLOT_H_
//#define _SPRITE2_SLOT_H_
//
//#include <SM_Vector.h>
//
//#include <CU_RefCountObj.h>
//
//#include <string>
//#include <vector>
//
//namespace s2
//{
//
//class Bone;
//class Sprite;
//
//class Slot : public cu::RefCountObj
//{
//public:
//	Slot();
//	Slot(const Slot& slot);
//	Slot& operator = (const Slot& slot);
//	~Slot();
//
//	void Draw(const RenderParams& params) const;
//
//private:
//	struct Attachment
//	{
//		Sprite* spr;
//		sm::vec2 pos;
//		float angle;
//
//		Attachment();
//		~Attachment();
//
//	}; // Attachment
//
//private:
//	std::string m_name;
//	Bone* m_bone;
//	std::vector<Attachment> m_attachments;
//
//}; // Slot
//
//}
//
//#endif // _SPRITE2_SLOT_H_