#ifndef _SPRITE2_MACRO_H_
#define _SPRITE2_MACRO_H_

#define S2_FLAG_METHOD(name, bit) \
	bool Is##name() const { \
		return (m_flags & bit) != 0; \
	} \
	void Set##name(bool flag) const { \
		if (flag) { \
			m_flags |= bit; \
		} else { \
			m_flags &= ~bit; \
		} \
	}

#endif // _SPRITE2_MACRO_H_