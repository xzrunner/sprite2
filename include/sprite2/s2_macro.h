#ifndef _SPRITE2_MACRO_H_
#define _SPRITE2_MACRO_H_

#define SPRITE_CLONE_FUNC(class) \
	public: \
		auto Clone() const { return std::static_pointer_cast<class, Sprite>(CloneImpl()); } \
	private: \
		virtual SprPtr CloneImpl() const { return std::make_shared<class>(*this); }

#endif // _SPRITE2_MACRO_H_