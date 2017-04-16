#include "Flat.h"
#include "Blob.h"
#include "S2_Symbol.h"
#include "S2_Sprite.h"
#include "ImageSprite.h"
#include "SymType.h"

namespace s2
{

Flat::Flat()
	: m_curr_blob(NULL)
{
}

Flat::~Flat()
{
	for (int i = 0, n = m_blobs.size(); i < n; ++i) {
		delete m_blobs[i];
	}	
	if (m_curr_blob) {
		delete m_curr_blob;
	}
}

void Flat::Add(const Sprite* spr, const sm::mat4& mat)
{
	int type = spr->GetSymbol()->Type();
	if (type == SYM_IMAGE) {
		if (!m_curr_blob) {
			m_curr_blob = new Blob;
		}
		m_curr_blob->Add(VI_DOWNCASTING<const ImageSprite*>(spr), mat);
	} else {
		if (m_curr_blob) {
			m_blobs.push_back(m_curr_blob);
			m_curr_blob = NULL;
		}
		m_blobs.push_back(new Blob(spr, mat));
	}
}

void Flat::Finish()
{
	if (m_curr_blob) {
		m_blobs.push_back(m_curr_blob);
		m_curr_blob = NULL;
	}
}

}