#include "StatImages.h"

#include <unirender/UR_RenderContext.h>
#include <unirender/ur_typedef.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

SINGLETON_DEFINITION(StatImages);

StatImages::StatImages()
{
	m_count = 0;
	m_memory = 0;
	m_4096_count = 0;
}

void StatImages::Add(int width, int height, int type)
{
	int bpp = GetTexBPP(type);
	++m_count;
	m_memory += width * height * bpp / 8 / 1024.0f / 1024.0f;

	if (width == 4096 && height == 4096) {
		++m_4096_count;
	}
}

void StatImages::Remove(int width, int height, int type)
{
	int bpp = GetTexBPP(type);
	--m_count;
	m_memory -= width * height * bpp / 8 / 1024.0f / 1024.0f;

	if (width == 4096 && height == 4096) {
		--m_4096_count;
	}
}

void StatImages::Print(std::string& str) const
{
	static char buf[512];
	sprintf(buf, "Images: n %d, mem %.1f, nmax %d\n", 
		m_count, m_memory, m_4096_count);
	str += buf;
}

int StatImages::GetTexBPP(int type)
{
	int bpp = 0;
	switch (type)
	{
	case ur::TEXTURE_RGBA4:
		bpp = 16;
		break;
	case ur::TEXTURE_RGBA8:
		bpp = 32;
		break;
	case ur::TEXTURE_PVR2:
#if defined( __APPLE__ ) && !defined(__MACOSX)
		bpp = 2;
#endif
		break;
	case ur::TEXTURE_PVR4:
#if defined( __APPLE__ ) && !defined(__MACOSX)
		bpp = 4;
#else
		bpp = 32;
#endif
		break;
	case ur::TEXTURE_ETC2:
#ifdef __ANDROID__
		bpp = 8;
#elif defined( __APPLE__ ) && !defined(__MACOSX)
		bpp = 8;
#elif defined _WIN32
		ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
		if (rc->IsSupportETC2()) {
			bpp = 8;
		} else {
			bpp = 32;
		}
#else
		bpp = 32;
#endif // __ANDROID__
		break;
	}
	return bpp;
}

}