#ifndef _SPRITE2_MACRO_H_
#define _SPRITE2_MACRO_H_

#include "sprite2/config.h"

#include <memmgr/Allocator.h>

#ifdef S2_VIRTUAL_INHERITANCE
	#define S2_SPR_CLONE_FUNC(class) \
		public: \
			auto Clone() const { return std::dynamic_pointer_cast<class, s2::Sprite>(CloneImpl()); } \
		private: \
			virtual s2::SprPtr CloneImpl() const override { return std::allocate_shared<class, mm::Allocator<class>>(mm::Allocator<class>(), *this); }
#else
	#define S2_SPR_CLONE_FUNC(class) \
		public: \
			auto Clone() const { return std::static_pointer_cast<class, s2::Sprite>(CloneImpl()); } \
		private: \
			virtual s2::SprPtr CloneImpl() const override { return std::allocate_shared<class, mm::Allocator<class>>(mm::Allocator<class>(), *this); }
#endif // S2_VIRTUAL_INHERITANCE

#endif // _SPRITE2_MACRO_H_