#include "StatImages.h"

#include <unirender/UR_RenderContext.h>
#include <unirender/ur_typedef.h>
#include <shaderlab/ShaderMgr.h>

#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

namespace s2
{

SINGLETON_DEFINITION(StatImages);

StatImages::StatImages()
{
	m_count = 0;
	m_memory = 0;
	m_4096_count = 0;
}

void StatImages::Add(int id, int width, int height, int type)
{
	++m_count;

	int bpp = GetTexBPP(type);
	float mem = width * height * bpp / 8 / 1024.0f / 1024.0f;
	m_memory += mem;

	if (width == 4096 && height == 4096) {
		++m_4096_count;
	}

	std::map<int, float>::iterator itr = m_id2mem.find(id);
	if (itr == m_id2mem.end()) {
		m_id2mem.insert(std::make_pair(id, mem));
	} else {
		itr->second += mem;
	}
}

void StatImages::Remove(int id, int width, int height, int type)
{
	--m_count;

	int bpp = GetTexBPP(type);
	float mem = width * height * bpp / 8 / 1024.0f / 1024.0f;
	m_memory -= mem;

	if (width == 4096 && height == 4096) {
		--m_4096_count;
	}

	std::map<int, float>::iterator itr = m_id2mem.find(id);
	assert(itr != m_id2mem.end());
	itr->second -= mem;
	if (fabs(itr->second) < FLT_EPSILON) {
		m_id2mem.erase(itr);
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