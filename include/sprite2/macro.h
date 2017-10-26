#ifndef _SPRITE2_MACRO_H_
#define _SPRITE2_MACRO_H_

#include <memmgr/Allocator.h>

#define SPRITE_CLONE_FUNC(class) \
	public: \
		auto Clone() const { return std::static_pointer_cast<class, Sprite>(CloneImpl()); } \
	private: \
		virtual SprPtr CloneImpl() const { return std::allocate_shared<class, mm::Allocator<class>>(mm::Allocator<class>(), *this); }

#endif // _SPRITE2_MACRO_H_