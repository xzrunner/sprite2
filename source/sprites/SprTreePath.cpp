#include "SprTreePath.h"
#include "STPBufPool.h"

#include <iostream>

namespace s2
{

SprTreePath::SprTreePath()
	: m_val(0)
#ifdef SPR_TREE_PATH_STATIC
	, m_num(0)
#else
	, m_buf(NULL)
#endif // SPR_TREE_PATH_STATIC
{
#ifdef S2_SPR_TREE_DEBUG
	m_name_paths.reserve(16);
	m_sym_paths.reserve(16);
#endif // S2_SPR_TREE_DEBUG
}

SprTreePath::SprTreePath(const SprTreePath& path)
	: m_val(path.m_val)
#ifdef SPR_TREE_PATH_STATIC
	, m_num(path.m_num)
#else
	, m_buf(NULL)
#endif // SPR_TREE_PATH_STATIC
#ifdef S2_SPR_TREE_DEBUG
	, m_name_paths(path.m_name_paths)
	, m_sym_paths(path.m_sym_paths)
#endif // S2_SPR_TREE_DEBUG
{
#ifdef SPR_TREE_PATH_STATIC
	memcpy(m_ids, path.m_ids, sizeof(int) * m_num);
#else
	if (path.m_buf) {
		m_buf = STPBufPool::Instance()->Fetch(path.m_buf->cap);
		m_buf->CopyFrom(path.m_buf);
	}
#endif // SPR_TREE_PATH_STATIC
}

SprTreePath& SprTreePath::operator = (const SprTreePath& path)
{
	m_val = path.m_val;

#ifdef SPR_TREE_PATH_STATIC
	m_num = path.m_num;
	memcpy(m_ids, path.m_ids, sizeof(int) * m_num);
#else
	STPBufPool* pool = STPBufPool::Instance();
	if (m_buf) 
	{
		if (path.m_buf)
		{
			if (m_buf->cap != path.m_buf->cap) {
				pool->Return(m_buf);
				m_buf = pool->Fetch(path.m_buf->cap);
			}
			m_buf->CopyFrom(path.m_buf);
		}
		else
		{
			STPBufPool::Instance()->Return(m_buf);
			m_buf = NULL;
		}
	}
	else
	{
		if (path.m_buf) {
			m_buf = pool->Fetch(path.m_buf->cap);
			m_buf->CopyFrom(path.m_buf);
		}
	}
#endif // SPR_TREE_PATH_STATIC

#ifdef S2_SPR_TREE_DEBUG
	m_name_paths = path.m_name_paths;
	m_sym_paths = path.m_sym_paths;
#endif // S2_SPR_TREE_DEBUG

	return *this;
}

SprTreePath::~SprTreePath()
{
#ifndef SPR_TREE_PATH_STATIC
	if (m_buf) {
		STPBufPool::Instance()->Return(m_buf);
	}
#endif // SPR_TREE_PATH_STATIC
}

#ifndef SPR_TREE_PATH_STATIC

void SprTreePath::Push(int spr_id)
{
	if (!m_buf)
	{
		m_buf = STPBufPool::Instance()->Fetch(2);
	}
	else if (m_buf->num >= m_buf->cap)
	{
		STPBuffer* new_buf = STPBufPool::Instance()->Fetch(m_buf->cap * 2);
		new_buf->CopyFrom(m_buf);
		STPBufPool::Instance()->Return(m_buf);
		m_buf = new_buf;
	}

	m_buf->ids[m_buf->num++] = spr_id;
	m_val += spr_id * m_buf->num;
}

void SprTreePath::Push(const Sprite& spr) 
{
	Push(spr.GetID());
#ifdef S2_SPR_TREE_DEBUG
	m_name_paths.push_back(spr.GetName());
	m_sym_paths.push_back(spr.GetSymbol()->GetID());
#endif // S2_SPR_TREE_DEBUG
}

#endif // SPR_TREE_PATH_STATIC

std::ostream & operator << (std::ostream& os, const SprTreePath& path)
{
	os << "[path: ";
#ifdef SPR_TREE_PATH_STATIC
	for (int i = 0, n = path.m_num; i < n; ++i) {
		os << path.m_ids[i] << ", ";
	}
#else
	for (int i = 0, n = path.m_buf->num; i < n; ++i) {
		os << path.m_buf->ids[i] << ", ";
	}
#endif // SPR_TREE_PATH_STATIC
	os << "]";
	return os;
}

}